#include "camera_win.hpp"

#if BNB_OS_WINDOWS

    #include "camera_device.hpp"

    #include <bnb/utils/internal/log/logged_object.hpp>
    #include <bnb/utils/internal/log/log_configurator.hpp>
    #include <bnb/utils/exceptions.hpp>

namespace
{
    bnb::logger logging("win_based_camera");
}

struct bnb::camera_win::impl
{
    impl(size_t index)
    {
        CoInitialize(nullptr);
        ComPtr<IMFMediaSource> ptr_media_source;
        HRESULT hr = CreateVideoDeviceSource(&ptr_media_source, index);
        if (FAILED(hr)) {
            BNB_THROW(std::runtime_error, "Failed to crate media source");
        }
        wrapped = std::make_unique<VideoCaptureDeviceMFWin>(ptr_media_source);
    }
    std::unique_ptr<VideoCaptureDeviceMFWin> wrapped = nullptr;
};


bnb::camera_win::camera_win(const camera_base::push_frame_cb_t& cb, size_t index)
    : camera_base(cb)
    , m_impl(std::make_unique<impl>(index))
{
    int camera_width = 0;
    int camera_height = 0;
    constexpr auto frames_per_second = 30;

    using resolution_size = std::pair<uint16_t, uint16_t>;
    using resolution = std::pair<std::string, resolution_size>;
    const std::list<resolution> resolutions{
        {"HD/WXGA", {1280, 720}},
        {"XGA", {1024, 768}},
        {"SVGA", {800, 600}},
        {"VGA", {640, 480}},
        {"HVGA", {480, 320}}};

    VideoPixelFormat pixel_format = PIXEL_FORMAT_NV12;

    HRESULT init_result = m_impl->wrapped->Init();
    if (FAILED(init_result)) {
        BNB_THROW(std::runtime_error, "Failed to init camera");
    }

    for (const auto& r : resolutions) {
        auto [width, height] = r.second;
        if (m_impl->wrapped->AllocateAndStart(width, height, frames_per_second, pixel_format) != S_OK) {
            continue;
        }

        camera_width = width;
        camera_height = height;

        logging.info("Selected %s camera resolution: %dx%d", r.first.c_str(), width, height);

        break;
    }

    if (camera_width == 0 || camera_height == 0) {
        BNB_THROW(std::runtime_error, "Failed to start camera");
    }

    m_impl->wrapped->SetCallback([this, camera_width, camera_height, pixel_format](std::shared_ptr<ScopedBufferLock> lock) {
        auto data = static_cast<color_plane_data_t*>(lock->data());
        auto y_plane_size = camera_width * camera_height;
        auto u_plane_size = camera_width * camera_height / 4;

        color_plane y_plane = color_plane(data, [lock](color_plane_data_t*) { /* DO NOTHING */ });

        bnb::image_format format(camera_width, camera_height, camera_orientation::deg_0, true, 0);
        yuv_format_t yuv_format;
        yuv_format.range = color_range::full;
        yuv_format.standard = color_std::bt601;

        yuv_image_t yuv_image;
        if (pixel_format == PIXEL_FORMAT_NV12) {
            yuv_format.format = yuv_format::yuv_nv12;

            auto uv_plane = color_plane(data + y_plane_size, [lock](color_plane_data_t*) { /* DO NOTHING */ });
            yuv_image = yuv_image_t(y_plane, uv_plane, format, yuv_format);
        } else if (pixel_format == PIXEL_FORMAT_I420) {
            yuv_format.format = yuv_format::yuv_i420;

            auto u_plane = color_plane(data + y_plane_size, [lock](color_plane_data_t*) { /* DO NOTHING */ });
            auto v_plane = color_plane(data + y_plane_size + u_plane_size, [lock](color_plane_data_t*) { /* DO NOTHING */ });
            yuv_image = yuv_image_t(y_plane, u_plane, v_plane, format, yuv_format);
        } else {
            BNB_THROW(std::runtime_error, "Unsupported format from camera");
        }

        full_image_t full_image(std::move(yuv_image));

        if (m_push_frame_cb) {
            m_push_frame_cb(std::move(full_image));
        }
    });
}

bnb::camera_win::~camera_win()
{
    HRESULT stop_result = m_impl->wrapped->Stop();
    if (FAILED(stop_result)) {
        logging.warning("Failed to stop camera device in destructor, unexpected behavior is possible");
    }
}

void bnb::camera_win::set_device_by_index(uint32_t index)
{
    logging.warning("Not enough metadata for camera_win");
}
void bnb::camera_win::set_device_by_id(const std::string& device_id)
{
    logging.warning("Not enough metadata for camera_win");
}

void bnb::camera_win::start()
{
    logging.warning("camera_win starts in constructor");
}

bnb::camera_sptr bnb::create_camera_device(camera_base::push_frame_cb_t cb, size_t index)
{
    return std::make_shared<bnb::camera_win>(cb, index);
}

#endif