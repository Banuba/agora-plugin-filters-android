#include "BanubaVideoProcessor.h"
#include <chrono>
#include <optional>
#include <utility>
#include <lib_yuv/libyuv.h>

namespace agora::extension {

    BanubaVideoProcessor::BanubaVideoProcessor(std::vector<std::string> path_to_resources,
                                               std::string client_token,
                                               int32_t width,
                                               int32_t height
    ) : m_path_to_resources(std::move(path_to_resources)), m_client_token(std::move(client_token)) {
        auto ort = std::make_shared<bnb::offscreen_render_target>(width, height);
        m_oep = bnb::interfaces::offscreen_effect_player::create(
                m_path_to_resources,
                m_client_token,
                width, height,
                ort);
        m_oep->enable_audio(false);

        image_format.width = static_cast<uint32_t>(width);
        image_format.height = static_cast<uint32_t>(height);
        image_format.orientation = bnb::camera_orientation::deg_0;

        y_plane = std::vector<std::uint8_t>(width * height);
        uv_plane = std::vector<std::uint8_t>((width / 2) * (height / 2) * 2);
    }

    void BanubaVideoProcessor::process_frame(
            const agora::media::base::VideoFrame &capturedFrame
    ) {
#ifdef DEBUG
        std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();
#endif
        libyuv::I420ToNV12(capturedFrame.yBuffer, capturedFrame.yStride,
                           capturedFrame.uBuffer, capturedFrame.uStride,
                           capturedFrame.vBuffer, capturedFrame.vStride,
                           y_plane.data(), image_format.width,
                           uv_plane.data(), image_format.width,
                           image_format.width, image_format.height);
        auto yuv_image = bnb::full_image_t(
                bnb::yuv_image_t{
                        bnb::color_plane_vector(y_plane),
                        bnb::color_plane_vector(uv_plane),
                        image_format
                }
        );
        auto image_ptr = std::make_shared<bnb::full_image_t>(std::move(yuv_image));
        auto pb = m_oep->process_image(image_ptr, target_orient);
        auto image = pb->get_rgba();
        if (image.has_value()) {
            auto &rgba_image = image->get_data<bnb::bpc8_image_t>();
            libyuv::ABGRToI420(rgba_image.get_data(), image_format.width * 4,
                               capturedFrame.yBuffer, capturedFrame.yStride,
                               capturedFrame.uBuffer, capturedFrame.uStride,
                               capturedFrame.vBuffer, capturedFrame.vStride,
                               image_format.width, image_format.height);
        }
        pb->unlock();
#ifdef DEBUG
        std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
        auto time_result = std::chrono::duration_cast<std::chrono::milliseconds>(
                time_end - time_begin).count();
        sendEvent("processFrame",
                  std::string("Processing time ms: ").append(std::to_string(time_result)).c_str());
#endif
    }

    void BanubaVideoProcessor::set_parameter(
            const std::string &key,
            const std::string &parameter
    ) {
        if (key == "load_effect") {
            m_oep->load_effect(parameter);
        }

    }

    void BanubaVideoProcessor::send_event(const char *key, const char *data) {
        if (m_control != nullptr) {
            m_control->fireEvent(m_id, key, data);
        }
    }
}
