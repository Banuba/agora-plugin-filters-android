//
// Created by AArol on 1/24/18.
//


#include <bnb/spal/camera/video_mocker.hpp>
#include <bnb/types/constants.hpp>
#include <bnb/utils/assert.hpp>

#include <boost/format.hpp>
#include <opencv2/imgproc.hpp>

using namespace bnb;

camera_video_mocker::camera_video_mocker(const camera_base::push_frame_cb_t& cb, const std::string& input_file, const rect_t& clip_area)
    : camera_base_mocker(cb)
    , m_capture(input_file)
    , m_clip_area(clip_area)
    , m_frame_width(0)
    , m_frame_height(0)
    , m_frame_size(0)
{
    if (!m_capture.isOpened()) {
        throw std::runtime_error((boost::format("Unable to open %s video file for capture") % input_file).str());
    }

    m_frame_width = static_cast<uint32_t>(m_capture.get(cv::CAP_PROP_FRAME_WIDTH));
    m_frame_height = static_cast<uint32_t>(m_capture.get(cv::CAP_PROP_FRAME_HEIGHT));

    // check clip area fits the image
    if (m_clip_area) {
        m_clip_area = *m_clip_area & cv::Rect(0, 0, m_frame_width, m_frame_height);

        // align both dimensions to size of 2
        (*m_clip_area).width &= ~1;
        (*m_clip_area).height &= ~1;

        m_frame_height = static_cast<uint32_t>((*m_clip_area).height);
        m_frame_width = static_cast<uint32_t>((*m_clip_area).width);
    }

    m_frame_size = m_frame_width * m_frame_height;
    BNB_ASSERT(m_frame_size > 0);

    auto native_frame_rate = static_cast<unsigned>(m_capture.get(cv::CAP_PROP_FPS));
    set_frame_rate(native_frame_rate);

    start();
}

camera_video_mocker::camera_video_mocker(const camera_base::push_frame_cb_t& cb, uint32_t frame_rate, const std::string& input_file, const rect_t& clip_area)
    : camera_video_mocker(cb, input_file, clip_area)
{
    set_frame_rate(frame_rate);
    //thread is already created here
}

camera_video_mocker::~camera_video_mocker()
{
    abort();

    if (m_capture.isOpened())
        m_capture.release();
}

void camera_video_mocker::update_image_content()
{
    static cv::Mat frame;
    m_capture >> frame;

    if (frame.empty()) {
        camera_base::abort();
        return;
    }

    // clip source video frame with provided clipping area
    if (m_clip_area)
        frame = frame(m_clip_area.value_or(cv::Rect()));

    cv::cvtColor(frame, frame, cv::COLOR_BGRA2YUV_IYUV);

    std::vector<unsigned char> y_plane_vector;
    std::vector<unsigned char> uv_plane_vector;
    y_plane_vector.assign(frame.data, frame.data + m_frame_height * m_frame_width);

    for (size_t i = 0; i < m_frame_height * m_frame_width / 4; ++i) {
        uv_plane_vector.emplace_back(frame.data[i + m_frame_height * m_frame_width]);
        uv_plane_vector.emplace_back(frame.data[i + m_frame_height * m_frame_width + m_frame_height * m_frame_width / 4]);
    }

    auto y_plane = color_plane_vector(std::move(y_plane_vector));
    auto uv_plane = color_plane_vector(std::move(uv_plane_vector));

    bnb::image_format image_format;
    image_format.orientation = camera_orientation::deg_0;
    image_format.require_mirroring = true;
    image_format.width = m_frame_width;
    image_format.height = m_frame_height;

    bnb::yuv_image_t yuv(std::move(y_plane), std::move(uv_plane), image_format);
    bnb::full_image_t image(std::move(yuv));

    m_push_frame_cb(std::move(image));
}

void camera_video_mocker::set_device_by_index(uint32_t)
{
    throw std::runtime_error("not implemented");
}

void camera_video_mocker::set_device_by_id(const std::string&)
{
    throw std::runtime_error("not implemented");
}

void camera_video_mocker::start()
{
    throw std::runtime_error("not implemented");
}
