
#pragma once

#include <chrono>
#include <thread>
#include <optional>

#include <bnb/types/base_types.hpp>
#include <bnb/spal/camera/base_mocker.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>


namespace bnb
{
    class BNB_EXPORT camera_video_mocker final : public camera_base_mocker
    {
    public:
        typedef std::optional<cv::Rect> rect_t;

    public:
        camera_video_mocker(const camera_base::push_frame_cb_t& cb, uint32_t frame_rate, const std::string& input_file, const rect_t& clip_area);
        camera_video_mocker(const camera_base::push_frame_cb_t& cb, const std::string& input_file, const rect_t& clip_area = std::nullopt);
        ~camera_video_mocker() final;

        void set_device_by_index(uint32_t device_index) override;
        void set_device_by_id(const std::string& device_id) override;
        void start() override;

    private:
        void update_image_content() final;

    private:
        uint32_t m_frame_width;
        uint32_t m_frame_height;
        uint32_t m_frame_size;

        cv::VideoCapture m_capture;
        std::optional<cv::Rect> m_clip_area;
    };

} // namespace bnb
