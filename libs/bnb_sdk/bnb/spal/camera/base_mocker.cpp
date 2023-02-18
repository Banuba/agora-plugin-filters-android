#include <bnb/spal/camera/base_mocker.hpp>
#include <bnb/types/constants.hpp>

#include <bnb/types/base_types.hpp>

#include <bnb/utils/std/filesystem.hpp>

#include <chrono>

using namespace bnb;

camera_base_mocker::camera_base_mocker(const camera_base::push_frame_cb_t& cb, uint32_t frame_rate)
    : camera_base(cb)
    , m_frame_rate_sleep(1000000 / frame_rate)
{
}

void camera_base_mocker::start()
{
    m_thread = std::thread([this]() { thread_func(); });
}


void camera_base_mocker::set_frame_rate(uint32_t frame_rate)
{
    m_frame_rate_sleep = std::chrono::microseconds(1000000 / frame_rate);
}


camera_base_mocker::~camera_base_mocker()
{
    camera_base::abort();
    if (m_thread.joinable())
        m_thread.join();
}

void camera_base_mocker::thread_func()
{
    while (!m_is_abort_requested) {
        auto start = std::chrono::high_resolution_clock::now();

        update_image_content();

        auto end = std::chrono::high_resolution_clock::now();
        auto call_duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        micro_sleep_t to_sleep = m_frame_rate_sleep - call_duration;
        std::this_thread::sleep_for(std::max(to_sleep, micro_sleep_t::zero()));
    }
}