
#pragma once

#include <bnb/types/base_types.hpp>
#include <bnb/spal/camera/base.hpp>

#if !BNB_OS_MACOS
    #include <opencv2/videoio.hpp>
    #include <opencv2/videoio/videoio_c.h>

    #include <thread>
    #include <mutex>

namespace bnb
{
    class BNB_EXPORT camera_ocv : public camera_base
    {
    public:
        explicit camera_ocv(const camera_base::push_frame_cb_t& cb, size_t index);
        ~camera_ocv() override;

        void set_device_by_index(uint32_t device_index) override;
        void set_device_by_id(const std::string& device_id) override;
        void start() override;

    private:
        void thread_func();

        void update_image_content();

        cv::VideoCapture m_capture;

        std::thread m_thread;

        uint32_t m_frame_width{0};
        uint32_t m_frame_height{0};
        uint32_t m_frame_size = m_frame_width * m_frame_height;
        uint32_t m_device_count{0};
        std::mutex m_mutex;
    };
} // namespace bnb
#endif
