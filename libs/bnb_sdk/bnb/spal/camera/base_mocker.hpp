#pragma once

#include <bnb/types/base_types.hpp>
#include <bnb/spal/camera/base.hpp>

#include <thread>
#include <string>

namespace bnb
{
    class BNB_EXPORT camera_base_mocker : public camera_base
    {
    public:
        using camera_base::push_frame_cb_t;

        explicit camera_base_mocker(const push_frame_cb_t& cb, uint32_t frame_rate = 30);
        ~camera_base_mocker() override;

        /*
         * Main thread
         */
        void set_frame_rate(uint32_t frame_rate);

    protected:
        virtual void update_image_content() = 0;
        void start() override;

    private:
        using micro_sleep_t = std::chrono::duration<long, std::micro>;
        micro_sleep_t m_frame_rate_sleep;

        void thread_func();

        std::thread m_thread;
    };

} // namespace bnb
