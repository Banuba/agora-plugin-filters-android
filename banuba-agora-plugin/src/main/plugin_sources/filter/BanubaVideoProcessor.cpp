#include "BanubaVideoProcessor.h"

#include <bnb/recognizer/interfaces/utility_manager.hpp>

#include <optional>

namespace agora::extension {

    void BanubaVideoProcessor::process_frame(
            const agora_refptr<rtc::IVideoFrame> &frame
    ) {
        auto oep = m_oep;
        if (!oep) {
            m_control->deliverVideoFrame(frame);
            return;
        }

        agora::rtc::VideoFrameData captured_frame;
        frame->getVideoFrameData(captured_frame);
        update_oep_surface_size(captured_frame.width, captured_frame.height);
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

        auto proc_callback = [this, frame, captured_frame] (const image_processing_result_sptr& result) {
            if (!result) {
                return;
            }
            auto image_callback = [this, frame, captured_frame] (const pixel_buffer_sptr& out_img) {
                // returned image may have stride != width, but agora doesn't provide
                // the way to set stride (expects stride == width), so copy image line by line
                uint8_t* src_y = out_img->get_base_sptr_of_plane(0).get();
                uint8_t* src_uv = out_img->get_base_sptr_of_plane(1).get();
                int32_t src_y_stride = out_img->get_bytes_per_row_of_plane(0);
                int32_t src_uv_stride = out_img->get_bytes_per_row_of_plane(1);
                int32_t src_y_size = src_y_stride * out_img->get_height();
                int32_t src_uv_size = src_uv_stride * out_img->get_height() / 2;

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

                m_control->deliverVideoFrame(frame);
            };
            result->get_image(bnb::oep::interfaces::image_format::nv12_bt709_full, image_callback);
        };
        oep->process_image_async(img, m_oep_input_rotation, false, proc_callback, bnb::oep::interfaces::rotation::deg0);
    }

    void BanubaVideoProcessor::set_parameter(
            const std::string &key,
            const std::string &parameter
    ) {
        if (!m_is_initialized) {
            /* Banuba SDK not initialized */
            if (key == "set_resources_path") {
                m_path_to_resources = parameter;
            } else if (key == "set_effects_path") {
                m_path_to_effects = parameter;
            } else if (key == "set_client_token") {
                m_client_token = parameter;
            } else if (key == "initialize") {
                initialize();
            } else {

            }
            return;
        }

        if (!m_oep) {
            /* oep not created */
            if (key == "create") {
                create();
            } else {

            }
            return;
        }

        if (key == "set_device_orientation") {
            auto device_orientation_degrees =  std::stoi(parameter);
            m_oep_input_rotation = degrees_to_bnb_rotation(device_orientation_degrees);
        } else if (key == "pause") {
            m_oep->pause();
        } else if (key == "resume") {
            m_oep->resume();
        } else if (key == "load_effect") {
            m_oep->load_effect(parameter);
        } else if (key == "unload_effect") {
            m_oep->unload_effect();
        } else if (key == "destroy") {
            destroy();
        } else {

        }
    }

    void BanubaVideoProcessor::initialize() {
        const std::vector<std::string> resource_paths {m_path_to_resources, m_path_to_effects};
        bnb::interfaces::utility_manager::initialize(resource_paths, m_client_token);
        m_is_initialized = true;
    }

    void BanubaVideoProcessor::create() {
        // Frame size
        constexpr int32_t default_oep_width = 1280;
        constexpr int32_t default_oep_height = 720;

        auto rc = bnb::oep::interfaces::render_context::create();
        auto ort = bnb::oep::interfaces::offscreen_render_target::create(rc);
        auto ep = bnb::oep::interfaces::effect_player::create(default_oep_width, default_oep_height);
        m_oep = bnb::oep::interfaces::offscreen_effect_player::create(ep, ort, default_oep_width, default_oep_height);
        update_oep_surface_size(default_oep_width, default_oep_height);
    }

    void BanubaVideoProcessor::destroy() {
        m_oep->stop();
        m_oep = nullptr;
        bnb::interfaces::utility_manager::release();
    }

    void BanubaVideoProcessor::update_oep_surface_size(int32_t width, int32_t height) {
        if (m_oep_surface_width != width || m_oep_surface_height != height) {
            m_oep->surface_changed(width, height);
            m_oep_surface_width = width;
            m_oep_surface_height = height;
        }
    }

    bnb::oep::interfaces::rotation BanubaVideoProcessor::degrees_to_bnb_rotation(int degrees) {
        using ns = bnb::oep::interfaces::rotation;
        switch (degrees) {
            case 90:
                return ns::deg90;
            case 180:
                return ns::deg180;
            case 270:
                return ns::deg270;
            default:
                return ns::deg0;
        }
    }
}
