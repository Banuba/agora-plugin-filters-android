#include "BanubaVideoProcessor.h"

#include <bnb/recognizer/interfaces/utility_manager.hpp>

#include <optional>

namespace agora::extension
{

    const char* CALL_INITIALIZE = "initialize";
    const char* CALL_CREATE = "create";
    const char* CALL_DESTROY = "destroy";
    const char* CALL_LOAD_EFFECT = "load_effect";
    const char* CALL_UNLOAD_EFFECT = "unload_effect";
    const char* CALL_PAUSE = "pause";
    const char* CALL_RESUME = "resume";
    const char* CALL_SET_DEVICE_ORIENTATION = "set_device_orientation";
    const char* CALL_SET_RESOURCES_PATH = "set_resources_path";
    const char* CALL_SET_EFFECTS_PATH = "set_effects_path";
    const char* CALL_SET_CLIENT_TOKEN = "set_client_token";

    static bool bnb_sdk_is_initialized = false;

    static int device_orientation_degrees_to_front_camera_orientation_degrees(int degrees)
    {
        return (degrees + 270) % 360;
    }

    static bnb::oep::interfaces::rotation degrees_to_oep_rotation(int degrees)
    {
        switch (degrees) {
            case 0:
                return bnb::oep::interfaces::rotation::deg0;
            case 90:
                return bnb::oep::interfaces::rotation::deg90;
            case 180:
                return bnb::oep::interfaces::rotation::deg180;
            case 270:
                return bnb::oep::interfaces::rotation::deg270;
            default:
                assert(0 && "Wrong 'degrees' value. The degrees must be fixed value");
        }
        return bnb::oep::interfaces::rotation::deg0;
    }

    void BanubaVideoProcessor::process_frame(
        const agora_refptr<rtc::IVideoFrame>& frame
    )
    {
        auto oep = m_oep;
        auto agora_control = m_control;
        if (!agora_control) {
            return;
        }
        if (!oep || !m_is_effect_loaded) {
            agora_control->deliverVideoFrame(frame);
            return;
        }

        agora::rtc::VideoFrameData captured_frame;
        frame->getVideoFrameData(captured_frame);
        using nsr = bnb::oep::interfaces::rotation;
        bnb::oep::interfaces::rotation target_orient = nsr::deg0;
        bool need_flip = m_oep_input_rotation == nsr::deg90 || m_oep_input_rotation == nsr::deg270;
        if (need_flip) {
            update_oep_surface_size(captured_frame.height, captured_frame.width);
        } else {
            update_oep_surface_size(captured_frame.width, captured_frame.height);
        }

        auto pixels = captured_frame.pixels.data;
        int32_t y_size = captured_frame.width * captured_frame.height;
        int32_t uv_size = y_size / 2;

        uint8_t* y_plane_data{pixels};
        uint8_t* uv_plane_data = y_plane_data + y_size;

        // Agora doesn't provide the way to "return" processed frame from plugin, just to "modify" it.
        // But due to Banuba SDK specifics, the input frame should NOT be modified even after processing
        // (Banuba SDK keeps this buffer internally for future usage).
        // So the only way to avoid modification of data already passed to Banuba SDK is just a copying
        // source frame into another buffer (which will be used and stored by Banuba SDK).
        // TODO: hide this specific on OEP or even SDK side
        using ns = bnb::oep::interfaces::pixel_buffer;
        ns::plane_data y_plane{std::shared_ptr<uint8_t>(new uint8_t[y_size]), 0, captured_frame.width};
        memcpy(y_plane.data.get(), y_plane_data, y_size);
        ns::plane_data uv_plane{std::shared_ptr<uint8_t>(new uint8_t[uv_size]), 0, captured_frame.width};
        memcpy(uv_plane.data.get(), uv_plane_data, uv_size);
        std::vector<ns::plane_data> planes{y_plane, uv_plane};

        pixel_buffer_sptr img = ns::create(planes, bnb::oep::interfaces::image_format::nv12_bt709_full, captured_frame.width, captured_frame.height);

        /* Do not pass 'this' to access the 'm_oep' and 'm_control' class members.
         * You can stumble upon a case when oep is already released, but the callback has not
         * yet been completed - this will lead to an application crash */
        auto proc_callback = [agora_control, oep, frame, captured_frame](const image_processing_result_sptr& result) {
            if (!result) {
                return;
            }
            auto image_callback = [agora_control, frame, captured_frame](const pixel_buffer_sptr& out_img) {
                // returned image may have stride != width, but agora doesn't provide
                // the way to set stride (expects stride == width), so copy image line by line
                uint8_t* src_y = out_img->get_base_sptr_of_plane(0).get();
                uint8_t* src_uv = out_img->get_base_sptr_of_plane(1).get();
                int32_t src_y_stride = out_img->get_bytes_per_row_of_plane(0);
                int32_t src_uv_stride = out_img->get_bytes_per_row_of_plane(1);
                int32_t src_y_size = src_y_stride * out_img->get_height_of_plane(0);
                int32_t src_uv_size = src_uv_stride * out_img->get_height_of_plane(1);

                uint8_t* dst_y = captured_frame.pixels.data;
                uint8_t* dst_uv = captured_frame.pixels.data + captured_frame.width * captured_frame.height;

                // copy Y plane
                for (uint8_t* line = src_y; line != src_y + src_y_size; line += src_y_stride) {
                    memcpy(dst_y, line, captured_frame.width);
                    dst_y += captured_frame.width;
                }

                // copy UV plane
                for (uint8_t* line = src_uv; line != src_uv + src_uv_size; line += src_uv_stride) {
                    memcpy(dst_uv, line, captured_frame.width);
                    dst_uv += captured_frame.width;
                }

                agora_control->deliverVideoFrame(frame);
            };
            result->get_image(bnb::oep::interfaces::image_format::nv12_bt709_full, image_callback);
        };
        oep->process_image_async(img, m_oep_input_rotation, false, proc_callback, m_oep_input_rotation);
    }

    void BanubaVideoProcessor::set_parameter(
        const std::string& key,
        const std::string& parameter
    )
    {
        if (!bnb_sdk_is_initialized) {
            /* Banuba SDK is not initialized */
            if (key == CALL_SET_RESOURCES_PATH) {
                m_path_to_resources = parameter;
            } else if (key == CALL_SET_EFFECTS_PATH) {
                m_path_to_effects = parameter;
            } else if (key == CALL_SET_CLIENT_TOKEN) {
                m_client_token = parameter;
            } else if (key == CALL_INITIALIZE) {
                initialize();
            }
            return;
        }

        if (!m_oep) {
            /* oep not created */
            if (key == CALL_CREATE) {
                create();
            }
            return;
        }

        if (key == CALL_SET_DEVICE_ORIENTATION) {
            auto device_orientation_degrees = std::stoi(parameter);
            auto camera_orientation_degrees = device_orientation_degrees_to_front_camera_orientation_degrees(device_orientation_degrees);
            m_oep_input_rotation = degrees_to_oep_rotation(camera_orientation_degrees);
        } else if (key == CALL_PAUSE) {
            m_oep->pause();
        } else if (key == CALL_RESUME) {
            m_oep->resume();
        } else if (key == CALL_LOAD_EFFECT) {
            m_oep->load_effect(parameter);
            m_is_effect_loaded = true;
        } else if (key == CALL_UNLOAD_EFFECT) {
            m_is_effect_loaded = false;
            m_oep->unload_effect();
        } else if (key == CALL_DESTROY) {
            m_is_effect_loaded = false;
            destroy();
        }
    }

    void BanubaVideoProcessor::initialize()
    {
        const std::vector<std::string> resource_paths{m_path_to_resources, m_path_to_effects};
        bnb::interfaces::utility_manager::initialize(resource_paths, m_client_token);
        bnb_sdk_is_initialized = true;
    }

    void BanubaVideoProcessor::create()
    {
        // Frame size
        constexpr int32_t default_oep_width = 1280;
        constexpr int32_t default_oep_height = 720;

        auto rc = bnb::oep::interfaces::render_context::create();
        auto ort = bnb::oep::interfaces::offscreen_render_target::create(rc);
        auto ep = bnb::oep::interfaces::effect_player::create(default_oep_width, default_oep_height);
        m_oep = bnb::oep::interfaces::offscreen_effect_player::create(ep, ort, default_oep_width, default_oep_height);
        update_oep_surface_size(default_oep_width, default_oep_height);
    }

    void BanubaVideoProcessor::destroy()
    {
        m_oep->stop();
        m_oep = nullptr;
    }

    void BanubaVideoProcessor::update_oep_surface_size(int32_t width, int32_t height)
    {
        if (m_oep_surface_width != width || m_oep_surface_height != height) {
            m_oep->surface_changed(width, height);
            m_oep_surface_width = width;
            m_oep_surface_height = height;
        }
    }

} // namespace agora::extension
