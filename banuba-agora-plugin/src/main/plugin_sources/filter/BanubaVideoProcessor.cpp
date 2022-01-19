#include "BanubaVideoProcessor.h"

#include <optional>

namespace agora::extension {

    BanubaVideoProcessor::BanubaVideoProcessor() = default;

    void BanubaVideoProcessor::process_frame(
            const agora_refptr<rtc::IVideoFrame> &frame
    ) {
        if (!m_is_initialized) {
            m_control->deliverVideoFrame(frame);
            return;
        }
        agora::rtc::VideoFrameData captured_frame;
        frame->getVideoFrameData(captured_frame);
        if (!m_oep) {
            create_ep(captured_frame.width, captured_frame.height);
        }
        auto pixels = captured_frame.pixels.data;
        int32_t y_size = captured_frame.width * captured_frame.height;

        uint8_t* y_plane_data{pixels};
        uint8_t* uv_plane_data = y_plane_data + y_size;

        using ns = bnb::oep::interfaces::pixel_buffer;
        ns::plane_data y_plane{std::shared_ptr<uint8_t>(y_plane_data, [frame](uint8_t*) { /* do nothing */ }), 0, captured_frame.width};
        ns::plane_data uv_plane{std::shared_ptr<uint8_t>(uv_plane_data, [frame](uint8_t*) { /* do nothing */ }), 0, captured_frame.width};
        std::vector<ns::plane_data> planes{y_plane, uv_plane};

        pixel_buffer_sptr img = ns::create(planes, bnb::oep::interfaces::image_format::nv12_bt709_full, captured_frame.width, captured_frame.height);

        auto proc_callback = [this, frame, captured_frame] (const image_processing_result_sptr& result) {
            if (result) {
                auto image_callback = [this, frame, captured_frame] (const pixel_buffer_sptr& out_img) {
                    int32_t y_size = captured_frame.width * captured_frame.height;
                    memcpy(captured_frame.pixels.data, out_img->get_base_sptr_of_plane(0).get(), y_size);
                    memcpy(captured_frame.pixels.data + y_size, out_img->get_base_sptr_of_plane(1).get(), y_size / 2);
                    m_control->deliverVideoFrame(frame);
                };
                result->get_image(bnb::oep::interfaces::image_format::nv12_bt709_full, image_callback);
            }
        };

        m_oep->process_image_async(img, bnb::oep::interfaces::rotation::deg0,
                                   proc_callback, std::nullopt);
    }

    void BanubaVideoProcessor::set_parameter(
            const std::string &key,
            const std::string &parameter
    ) {
        if (m_oep && key == "load_effect") {
            m_oep->load_effect(parameter);
            return;
        }
        if (key == "set_resources_path") {
            m_path_to_resources = parameter;
            initialize();
            return;
        }
        if (key == "set_effects_path") {
            m_path_to_effects = parameter;
            initialize();
            return;
        }
        if (key == "set_token") {
            m_client_token = parameter;
            initialize();
            return;
        }
    }

    void BanubaVideoProcessor::initialize() {
        if (m_client_token.empty() || m_path_to_resources.empty() ||
            m_path_to_effects.empty())
            return;
        m_ep = bnb::oep::interfaces::effect_player::create(
                {m_path_to_resources, m_path_to_effects}, m_client_token);
        m_is_initialized = true;
    }

    void BanubaVideoProcessor::create_ep(int32_t width, int32_t height) {
        auto rc = bnb::oep::interfaces::render_context::create();
        auto ort = bnb::oep::interfaces::offscreen_render_target::create(rc);
        m_oep = bnb::oep::interfaces::offscreen_effect_player::create(m_ep, ort, width, height);
        m_oep->surface_changed(width, height);
//        m_oep->enable_audio(false);
    }

    void BanubaVideoProcessor::send_event(const char *key, const char *data) {
        if (m_control) {
            m_control->postEvent(key, data);
        }
    }
}
