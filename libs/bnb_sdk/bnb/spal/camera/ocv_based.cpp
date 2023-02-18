//
// Created by Lechenko on 1/16/18.
//

#include <bnb/spal/camera/ocv_based.hpp>
#include <bnb/types/constants.hpp>
#include <bnb/types/base_types.hpp>
#include <bnb/types/conversion.hpp>
#include <bnb/utils/internal/profiler.hpp>
#include <bnb/utils/internal/log/logged_object.hpp>
#include <bnb/utils/internal/log/log_configurator.hpp>

#if !BNB_OS_MACOS && !BNB_OS_WINDOWS

    #include <opencv2/core/mat.hpp>
    #include <opencv2/imgproc.hpp>

    #include <utility>
    #include <list>

using namespace bnb;
namespace
{
    bnb::logger logging("ocv_based_camera");
}

camera_ocv::camera_ocv(const camera_base::push_frame_cb_t& cb, size_t index)
    : camera_base(cb)
    , m_capture(index)
    , m_thread()
{
    if (!m_capture.isOpened())
        throw std::runtime_error("Unable to open camera for capture");

    using resolution_size = std::pair<uint16_t, uint16_t>;
    using resolution = std::pair<std::string, resolution_size>;
    const std::list<resolution> resolutions{
        {"HD/WXGA", {1280, 720}},
        {"XGA", {1024, 768}},
        {"SVGA", {800, 600}},
        {"VGA", {640, 480}},
        {"HVGA", {480, 320}}};

    m_capture.set(cv::CAP_PROP_FPS, 30);

    for (const auto& r : resolutions) {
        auto [width, height] = r.second;

        m_capture.set(cv::CAP_PROP_FRAME_WIDTH, width);
        m_capture.set(cv::CAP_PROP_FRAME_HEIGHT, height);

        if ((m_capture.get(cv::CAP_PROP_FRAME_WIDTH) != width) || (m_capture.get(cv::CAP_PROP_FRAME_HEIGHT) != height)) {
            continue;
        }

        m_frame_width = width;
        m_frame_height = height;

        logging.info("Selected %s camera resolution: %dx%d", r.first.c_str(), width, height);

        break;
    }

    if (m_frame_width == 0 || m_frame_height == 0) {
        std::string res_list;
        for (const auto& r : resolutions) {
            auto [width, height] = r.second;
            res_list.append(r.first + ": " + std::to_string(width) + "x" + std::to_string(height) + "\r\n");
        }

        logging.warning("Camera doesn't support any value of the list of available resolutions: %s", res_list.c_str());

        auto cam_width = m_capture.get(cv::CAP_PROP_FRAME_WIDTH);
        auto cam_height = m_capture.get(cv::CAP_PROP_FRAME_HEIGHT);

        if (cam_width == 0 || cam_height == 0) {
            throw std::runtime_error("Unable to use camera");
        }

        m_frame_width = uint32_t(cam_width);
        m_frame_height = uint32_t(cam_height);
    }

    m_frame_size = m_frame_height * m_frame_width;

    m_thread = std::thread(&camera_ocv::thread_func, this);
}

void camera_ocv::set_device_by_index(uint32_t)
{
    logging.warning("Not enough metadata for camera_ocv");
}

void camera_ocv::set_device_by_id(const std::string&)
{
    logging.warning("Not enough metadata for camera_ocv");
}

void camera_ocv::start()
{
    logging.warning("camera_ocv starts in constructor");
}

camera_ocv::~camera_ocv()
{
    camera_base::abort();

    if (m_thread.joinable())
        m_thread.join();
    if (m_capture.isOpened())
        m_capture.release();
}

void camera_ocv::thread_func()
{
    while (true) {
        if (m_is_abort_requested)
            break;
        update_image_content();
    }
}

void camera_ocv::update_image_content()
{
    BNB_PRF_FUNCTION();
    std::lock_guard<std::mutex> lock(m_mutex);
    static cv::Mat image;

    if (!m_capture.isOpened() || !m_capture.read(image)) {
        return;
    }

    image_format format;
    format.orientation = camera_orientation::deg_0;
    format.require_mirroring = true;
    format.width = m_frame_width;
    format.height = m_frame_height;

    bpc8_image_t bpc_image(color_plane_weak(image.data), interfaces::pixel_format::bgr, format);
    full_image_t full_image(std::move(bpc_image));

    m_push_frame_cb(std::move(full_image));
}

camera_sptr bnb::create_camera_device(camera_base::push_frame_cb_t cb, size_t index)
{
    return std::make_shared<bnb::camera_ocv>(cb, index);
}
#endif
