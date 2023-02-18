// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Merged in single header MF based camera from Chromium project.

#pragma once

#include <bnb/utils/defs.hpp>

#if BNB_OS_WINDOWS

// clang-format off

#include <cassert>
#include <mutex>
#include <functional>
#include <atomic>
#include <mfapi.h>
#include <mferror.h>
#include <list>
#include <stddef.h>
#include <wincodec.h>

#include <thread>
#include <utility>
#include <wrl.h>


#include <Windows.h>
#pragma comment(lib,"Mfplat.lib")
#pragma comment(lib,"Mf.lib")
#pragma comment(lib,"Mfreadwrite.lib")
#pragma comment(lib,"mfuuid.lib")
#pragma comment(lib,"shlwapi.lib")

using Microsoft::WRL::ComPtr;

#include "mfcaptureengine.h"


template <class T> void SafeRelease(T** ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

enum VideoPixelFormat {
    PIXEL_FORMAT_UNKNOWN = 0,  // Unknown or unspecified format value.
    PIXEL_FORMAT_I420 =
    1,  // 12bpp YUV planar 1x1 Y, 2x2 UV samples, a.k.a. YU12.

// Note: Chrome does not actually support YVU compositing, so you probably
// don't actually want to use this. See http://crbug.com/784627.
PIXEL_FORMAT_YV12 = 2,  // 12bpp YVU planar 1x1 Y, 2x2 VU samples.

PIXEL_FORMAT_I422 = 3,   // 16bpp YUV planar 1x1 Y, 2x1 UV samples.
PIXEL_FORMAT_I420A = 4,  // 20bpp YUVA planar 1x1 Y, 2x2 UV, 1x1 A samples.
PIXEL_FORMAT_I444 = 5,   // 24bpp YUV planar, no subsampling.
PIXEL_FORMAT_NV12 =
6,  // 12bpp with Y plane followed by a 2x2 interleaved UV plane.
PIXEL_FORMAT_NV21 =
7,  // 12bpp with Y plane followed by a 2x2 interleaved VU plane.
PIXEL_FORMAT_UYVY =
8,  // 16bpp interleaved 2x1 U, 1x1 Y, 2x1 V, 1x1 Y samples.
PIXEL_FORMAT_YUY2 =
9,  // 16bpp interleaved 1x1 Y, 2x1 U, 1x1 Y, 2x1 V samples.
PIXEL_FORMAT_ARGB = 10,   // 32bpp BGRA (byte-order), 1 plane.
PIXEL_FORMAT_XRGB = 11,   // 24bpp BGRX (byte-order), 1 plane.
PIXEL_FORMAT_RGB24 = 12,  // 24bpp BGR (byte-order), 1 plane.

/* PIXEL_FORMAT_RGB32 = 13,  Deprecated */
PIXEL_FORMAT_MJPEG = 14,  // MJPEG compressed.
/* PIXEL_FORMAT_MT21 = 15,  Deprecated */

// The P* in the formats below designates the number of bits per pixel
// component. I.e. P9 is 9-bits per pixel component, P10 is 10-bits per pixel
// component, etc.
PIXEL_FORMAT_YUV420P9 = 16,
PIXEL_FORMAT_YUV420P10 = 17,
PIXEL_FORMAT_YUV422P9 = 18,
PIXEL_FORMAT_YUV422P10 = 19,
PIXEL_FORMAT_YUV444P9 = 20,
PIXEL_FORMAT_YUV444P10 = 21,
PIXEL_FORMAT_YUV420P12 = 22,
PIXEL_FORMAT_YUV422P12 = 23,
PIXEL_FORMAT_YUV444P12 = 24,

/* PIXEL_FORMAT_Y8 = 25, Deprecated */
PIXEL_FORMAT_Y16 = 26,  // single 16bpp plane.

PIXEL_FORMAT_ABGR = 27,  // 32bpp RGBA (byte-order), 1 plane.
PIXEL_FORMAT_XBGR = 28,  // 24bpp RGBX (byte-order), 1 plane.

PIXEL_FORMAT_P016LE = 29,  // 24bpp NV12, 16 bits per channel

PIXEL_FORMAT_XR30 =
30,  // 32bpp BGRX, 10 bits per channel, 2 bits ignored, 1 plane
PIXEL_FORMAT_XB30 =
31,  // 32bpp RGBX, 10 bits per channel, 2 bits ignored, 1 plane

PIXEL_FORMAT_BGRA = 32,  // 32bpp ARGB (byte-order), 1 plane.

// Please update UMA histogram enumeration when adding new formats here.
PIXEL_FORMAT_MAX =
PIXEL_FORMAT_BGRA,  // Must always be equal to largest entry logged.
};

// This list is ordered by precedence of use.
static VideoPixelFormat const kSupportedCapturePixelFormats[] = {
    PIXEL_FORMAT_NV12, PIXEL_FORMAT_I420,  PIXEL_FORMAT_YV12,
    PIXEL_FORMAT_NV21,  PIXEL_FORMAT_UYVY, PIXEL_FORMAT_YUY2,
    PIXEL_FORMAT_RGB24, PIXEL_FORMAT_ARGB, PIXEL_FORMAT_MJPEG,
};


namespace gfx {
    struct Size {
        int width_ = 0;
        int height_ = 0;
        void SetWidthAndHeight(int w, int h) {
            width_ = w;
            height_ = h;
        }
        int width() const { return width_; }
        int height() const { return height_; }
    };
}



struct VideoCaptureFormat {
    gfx::Size frame_size;
    float frame_rate = 0;
    VideoPixelFormat pixel_format = PIXEL_FORMAT_UNKNOWN;
};

HRESULT CreateCaptureEngine(IMFCaptureEngine** engine) {
    ComPtr<IMFCaptureEngineClassFactory> capture_engine_class_factory;
    HRESULT hr = CoCreateInstance(CLSID_MFCaptureEngineClassFactory, nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&capture_engine_class_factory));
    if (FAILED(hr))
        return hr;

    return capture_engine_class_factory->CreateInstance(CLSID_MFCaptureEngine,
        IID_PPV_ARGS(engine));
}

HRESULT CopyAttribute(IMFAttributes* source_attributes,
    IMFAttributes* destination_attributes,
    const GUID& key) {
    PROPVARIANT var;
    PropVariantInit(&var);
    HRESULT hr = source_attributes->GetItem(key, &var);
    if (FAILED(hr))
        return hr;

    hr = destination_attributes->SetItem(key, var);
    PropVariantClear(&var);
    return hr;
}

HRESULT CreateVideoDeviceSource(IMFMediaSource** ppSource, size_t index)
{
    UINT32 count = 0;

    *ppSource = NULL;

    IMFMediaSource* pSource = NULL;
    IMFAttributes* pAttributes = NULL;
    IMFActivate** ppDevices = NULL;

    // Create an attribute store to specify the enumeration parameters.
    HRESULT hr = MFCreateAttributes(&pAttributes, 1);
    if (FAILED(hr))
    {
        goto done;
    }

    // Source type: video capture devices
    hr = pAttributes->SetGUID(
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE,
        MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID
    );
    if (FAILED(hr))
    {
        goto done;
    }

    // Enumerate devices.
    hr = MFEnumDeviceSources(pAttributes, &ppDevices, &count);
    if (FAILED(hr))
    {
        goto done;
    }

    if (count == 0 || index >= count)
    {
        hr = E_FAIL;
        goto done;
    }

    // Create the media source object.
    hr = ppDevices[index]->ActivateObject(IID_PPV_ARGS(&pSource));
    if (FAILED(hr))
    {
        goto done;
    }

    *ppSource = pSource;
    (*ppSource)->AddRef();

done:
    SafeRelease(&pAttributes);

    for (DWORD i = 0; i < count; i++)
    {
        SafeRelease(&ppDevices[i]);
    }
    CoTaskMemFree(ppDevices);
    SafeRelease(&pSource);
    return hr;
}


bool GetFrameSizeFromMediaType(IMFMediaType* type, gfx::Size* frame_size) {
    UINT32 width32, height32;
    if (FAILED(MFGetAttributeSize(type, MF_MT_FRAME_SIZE, &width32, &height32)))
        return false;
    frame_size->SetWidthAndHeight(width32, height32);
    return true;
}

struct MediaFormatConfiguration {
    GUID mf_source_media_subtype;
    GUID mf_sink_media_subtype;
    VideoPixelFormat pixel_format;
};

bool GetMediaFormatConfigurationFromMFSourceMediaSubtype(
    const GUID& mf_source_media_subtype,
    MediaFormatConfiguration* media_format_configuration) {
    static const MediaFormatConfiguration kMediaFormatConfigurationMap[] = {
        // IMFCaptureEngine inevitably performs the video frame decoding itself.
        // This means that the sink must always be set to an uncompressed video
        // format.

        // Since chromium uses I420 at the other end of the pipe, MF known video
        // output formats are always set to I420.
        {MFVideoFormat_I420, MFVideoFormat_I420, PIXEL_FORMAT_I420},
        {MFVideoFormat_YUY2, MFVideoFormat_I420, PIXEL_FORMAT_I420},
        {MFVideoFormat_UYVY, MFVideoFormat_I420, PIXEL_FORMAT_I420},
        {MFVideoFormat_RGB24, MFVideoFormat_I420, PIXEL_FORMAT_I420},
        {MFVideoFormat_RGB32, MFVideoFormat_I420, PIXEL_FORMAT_I420},
        {MFVideoFormat_ARGB32, MFVideoFormat_I420, PIXEL_FORMAT_I420},
        {MFVideoFormat_MJPG, MFVideoFormat_I420, PIXEL_FORMAT_I420},
        {MFVideoFormat_NV12, MFVideoFormat_NV12, PIXEL_FORMAT_NV12},
        {MFVideoFormat_YV12, MFVideoFormat_I420, PIXEL_FORMAT_I420},

        /*
        // Depth cameras use 16-bit uncompressed video formats.
        // We ask IMFCaptureEngine to let the frame pass through, without
        // transcoding, since transcoding would lead to precision loss.
        {kMediaSubTypeY16, kMediaSubTypeY16, PIXEL_FORMAT_Y16},
        {kMediaSubTypeZ16, kMediaSubTypeZ16, PIXEL_FORMAT_Y16},
        {kMediaSubTypeINVZ, kMediaSubTypeINVZ, PIXEL_FORMAT_Y16},
        {MFVideoFormat_D16, MFVideoFormat_D16, PIXEL_FORMAT_Y16},
        */

        // Photo type
        {GUID_ContainerFormatJpeg, GUID_ContainerFormatJpeg, PIXEL_FORMAT_MJPEG} };

    for (const auto& kMediaFormatConfiguration : kMediaFormatConfigurationMap) {
        if (IsEqualGUID(kMediaFormatConfiguration.mf_source_media_subtype,
            mf_source_media_subtype)) {
            *media_format_configuration = kMediaFormatConfiguration;
            return true;
        }
    }

    return false;
}


bool GetPixelFormatFromMFSourceMediaSubtype(const GUID& mf_source_media_subtype, VideoPixelFormat* pixel_format) {
    MediaFormatConfiguration media_format_configuration;
    if (!GetMediaFormatConfigurationFromMFSourceMediaSubtype(
        mf_source_media_subtype, &media_format_configuration))
        return false;
    *pixel_format = media_format_configuration.pixel_format;
    return true;
}

// Calculate sink subtype based on source subtype. |passthrough| is set when
// sink and source are the same and means that there should be no transcoding
// done by IMFCaptureEngine.
HRESULT GetMFSinkMediaSubtype(IMFMediaType* source_media_type,
    GUID* mf_sink_media_subtype,
    bool* passthrough) {
    GUID source_subtype;
    HRESULT hr = source_media_type->GetGUID(MF_MT_SUBTYPE, &source_subtype);
    if (FAILED(hr))
        return hr;
    MediaFormatConfiguration media_format_configuration;
    if (!GetMediaFormatConfigurationFromMFSourceMediaSubtype(
        source_subtype, &media_format_configuration))
        return E_FAIL;
    *mf_sink_media_subtype = media_format_configuration.mf_sink_media_subtype;
    *passthrough =
        IsEqualGUID(media_format_configuration.mf_sink_media_subtype, source_subtype);
    return S_OK;
}

HRESULT ConvertToVideoSinkMediaType(IMFMediaType* source_media_type,
    IMFMediaType* sink_media_type, UINT32 frame_rate) {
    HRESULT hr = sink_media_type->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    if (FAILED(hr))
        return hr;

    bool passthrough = false;
    GUID mf_sink_media_subtype;
    hr = GetMFSinkMediaSubtype(source_media_type, &mf_sink_media_subtype,
        &passthrough);
    if (FAILED(hr))
        return hr;

    hr = sink_media_type->SetGUID(MF_MT_SUBTYPE, mf_sink_media_subtype);
    // Copying attribute values for passthrough mode is redundant, since the
    // format is kept unchanged, and causes AddStream error MF_E_INVALIDMEDIATYPE.
    if (FAILED(hr)/* || passthrough*/)
        return hr;

    hr = CopyAttribute(source_media_type, sink_media_type, MF_MT_FRAME_SIZE);
    if (FAILED(hr))
        return hr;

    // set reuqested frame rate
    hr = MFSetAttributeRatio(sink_media_type, MF_MT_FRAME_RATE, frame_rate, 1);
    if (FAILED(hr))
        return hr;
    ///

    hr = CopyAttribute(source_media_type, sink_media_type,
        MF_MT_PIXEL_ASPECT_RATIO);
    if (FAILED(hr))
        return hr;

    return CopyAttribute(source_media_type, sink_media_type,
        MF_MT_INTERLACE_MODE);
}


struct CapabilityWin {
    CapabilityWin(int media_type_index, const VideoCaptureFormat& format)
        : media_type_index(media_type_index),
        supported_format(format),
        info_header(),
        stream_index(0) {}

    // Used by VideoCaptureDeviceWin.
    CapabilityWin(int media_type_index,
        const VideoCaptureFormat& format,
        const BITMAPINFOHEADER& info_header)
        : media_type_index(media_type_index),
        supported_format(format),
        info_header(info_header),
        stream_index(0) {}

    // Used by VideoCaptureDeviceMFWin.
    CapabilityWin(int media_type_index,
        const VideoCaptureFormat& format,
        int stream_index)
        : media_type_index(media_type_index),
        supported_format(format),
        info_header(),
        stream_index(stream_index) {}

    const int media_type_index;
    const VideoCaptureFormat supported_format;

    // |info_header| is only valid if DirectShow is used.
    const BITMAPINFOHEADER info_header;

    // |stream_index| is only valid if MediaFoundation is used.
    const int stream_index;
};

typedef std::list<CapabilityWin> CapabilityList;


// static
bool ComparePixelFormatPreference(
    const VideoPixelFormat& lhs,
    const VideoPixelFormat& rhs) {
    auto* format_lhs = std::find(
        kSupportedCapturePixelFormats,
        kSupportedCapturePixelFormats + std::size(kSupportedCapturePixelFormats),
        lhs);
    auto* format_rhs = std::find(
        kSupportedCapturePixelFormats,
        kSupportedCapturePixelFormats + std::size(kSupportedCapturePixelFormats),
        rhs);
    return format_lhs < format_rhs;
}


// Compares the priority of the capture formats. Returns true if |lhs| is the
// preferred capture format in comparison with |rhs|. Returns false otherwise.
bool CompareCapability(const VideoCaptureFormat& requested,
    const VideoCaptureFormat& lhs,
    const VideoCaptureFormat& rhs) {
    // When 16-bit format is requested and available, avoid other formats.
    // If both lhs and rhs are 16-bit, we still need to compare them based on
    // height, width and frame rate.
    const bool use_requested =
        (requested.pixel_format == PIXEL_FORMAT_Y16);
    if (use_requested && lhs.pixel_format != rhs.pixel_format) {
        if (lhs.pixel_format == requested.pixel_format)
            return true;
        if (rhs.pixel_format == requested.pixel_format)
            return false;
    }
    const int diff_height_lhs =
        std::abs(lhs.frame_size.height() - requested.frame_size.height());
    const int diff_height_rhs =
        std::abs(rhs.frame_size.height() - requested.frame_size.height());
    if (diff_height_lhs != diff_height_rhs)
        return diff_height_lhs < diff_height_rhs;

    const int diff_width_lhs =
        std::abs(lhs.frame_size.width() - requested.frame_size.width());
    const int diff_width_rhs =
        std::abs(rhs.frame_size.width() - requested.frame_size.width());
    if (diff_width_lhs != diff_width_rhs)
        return diff_width_lhs < diff_width_rhs;

    const float diff_fps_lhs = std::fabs(lhs.frame_rate - requested.frame_rate);
    const float diff_fps_rhs = std::fabs(rhs.frame_rate - requested.frame_rate);
    if (diff_fps_lhs != diff_fps_rhs)
        return diff_fps_lhs < diff_fps_rhs;

    return ComparePixelFormatPreference(lhs.pixel_format,
        rhs.pixel_format);
}

const CapabilityWin& GetBestMatchedCapability(
    const VideoCaptureFormat& requested,
    const CapabilityList& capabilities) {

    const CapabilityWin* best_match = &(*capabilities.begin());
    for (const CapabilityWin& capability : capabilities) {
        if (CompareCapability(requested, capability.supported_format,
            best_match->supported_format)) {
            best_match = &capability;
        }
    }
    return *best_match;
}




// Locks the given buffer using the fastest supported method when constructed,
// and automatically unlocks the buffer when destroyed.
class ScopedBufferLock {
public:
    explicit ScopedBufferLock(ComPtr<IMFMediaBuffer> buffer)
        : buffer_(std::move(buffer)) {
        if (FAILED(buffer_.As(&buffer_2d_))) {
            LockSlow();
            return;
        }
        // Try lock methods from fastest to slowest: Lock2DSize(), then Lock2D(),
        // then finally LockSlow().
        if (Lock2DSize() || Lock2D()) {
            if (IsContiguous())
                return;
            buffer_2d_->Unlock2D();
        }
        // Fall back to LockSlow() if 2D buffer was unsupported or noncontiguous.
        buffer_2d_ = nullptr;
        LockSlow();
    }

    // Returns whether |buffer_2d_| is contiguous with positive pitch, i.e., the
    // buffer format that the surrounding code expects.
    bool IsContiguous() {
        BOOL is_contiguous;
        return pitch_ > 0 &&
            SUCCEEDED(buffer_2d_->IsContiguousFormat(&is_contiguous)) &&
            is_contiguous &&
            (length_ || SUCCEEDED(buffer_2d_->GetContiguousLength(&length_)));
    }

    bool Lock2DSize() {
        ComPtr<IMF2DBuffer2> buffer_2d_2;
        if (FAILED(buffer_.As(&buffer_2d_2)))
            return false;
        BYTE* data_start;
        return SUCCEEDED(buffer_2d_2->Lock2DSize(MF2DBuffer_LockFlags_Read, &data_,
            &pitch_, &data_start, &length_));
    }

    bool Lock2D() { return SUCCEEDED(buffer_2d_->Lock2D(&data_, &pitch_)); }

    void LockSlow() {
        DWORD max_length = 0;
        buffer_->Lock(&data_, &max_length, &length_);
    }

    ~ScopedBufferLock() {
        if (buffer_2d_)
            buffer_2d_->Unlock2D();
        else
            buffer_->Unlock();
    }

    ScopedBufferLock(const ScopedBufferLock&) = delete;
    ScopedBufferLock& operator=(const ScopedBufferLock&) = delete;

    BYTE* data() const { return data_; }
    DWORD length() const { return length_; }
    LONG pitch() const { return pitch_; }

private:
    ComPtr<IMFMediaBuffer> buffer_;
    ComPtr<IMF2DBuffer> buffer_2d_;
    BYTE* data_ = nullptr;
    DWORD length_ = 0;
    LONG pitch_ = 0;
};

template <typename T>
class MFVideoCallback final
    :
    public IMFCaptureEngineOnSampleCallback,
    public IMFCaptureEngineOnEventCallback {
public:
    MFVideoCallback(T* observer) : observer_(observer) {}

    IFACEMETHODIMP QueryInterface(REFIID riid, void** object) override {
        HRESULT hr = E_NOINTERFACE;
        if (IsEqualGUID(riid, IID_IUnknown)) {
            *object = this;
            hr = S_OK;
        }
        else if (IsEqualGUID(riid, IID_IMFCaptureEngineOnSampleCallback)) {
            *object = static_cast<IMFCaptureEngineOnSampleCallback*>(this);
            hr = S_OK;
        }
        else if (IsEqualGUID(riid, IID_IMFCaptureEngineOnEventCallback)) {
            *object = static_cast<IMFCaptureEngineOnEventCallback*>(this);
            hr = S_OK;
        }
        if (SUCCEEDED(hr))
            AddRef();

        return hr;
    }

    IFACEMETHODIMP_(ULONG) AddRef() override {
        return InterlockedIncrement(&m_cRef);
    }

    IFACEMETHODIMP_(ULONG) Release() override {
        ULONG l = InterlockedDecrement(&m_cRef);
        if (0 == l) delete this;
        return l;
    }

    IFACEMETHODIMP OnEvent(IMFMediaEvent* media_event) override {
        if (!observer_) {
            return S_OK;
        }

        GUID capture_event_guid = GUID_NULL;
        {
            std::lock_guard<std::mutex> guard(lock_);
            observer_->OnEvent(media_event);
            if (HRESULT hr = media_event->GetExtendedType(&capture_event_guid); FAILED(hr)) {
                return hr;
            }
        }
        return S_OK;
    }

    IFACEMETHODIMP OnSample(IMFSample* sample) override {
        std::lock_guard<std::mutex> guard(lock_);

        if (!observer_) {
            return S_OK;
        }
        if (!sample) {
            observer_->OnFrameDropped();
            return S_OK;
        }
        /*
        base::TimeTicks reference_time(base::TimeTicks::Now());
        LONGLONG raw_time_stamp = 0;
        sample->GetSampleTime(&raw_time_stamp);
        base::TimeDelta timestamp =
            base::TimeDelta::FromMicroseconds(raw_time_stamp / 10);
        */

        DWORD count = 0;
        sample->GetBufferCount(&count);

        for (DWORD i = 0; i < count; ++i) {
            ComPtr<IMFMediaBuffer> buffer;
            sample->GetBufferByIndex(i, &buffer);
            if (buffer) {
                auto locked_buffer = std::make_shared<ScopedBufferLock>(buffer);
                if (locked_buffer->data()) {
                    observer_->OnIncomingCapturedData(std::move(locked_buffer));
                }
                else {
                    observer_->OnFrameDropped();
                }
            }
            else {
                observer_->OnFrameDropped();
            }
        }
        return S_OK;
    }

    void Shutdown() {
        std::lock_guard<std::mutex> guard(lock_);
        observer_ = nullptr;
    }

private:
    ULONG m_cRef = 0;
    ~MFVideoCallback() {}
    std::mutex lock_;
    T* observer_;
};

class VideoCaptureDeviceMFWin {
public:
    using callback_t = std::function<void(std::shared_ptr<ScopedBufferLock> lock)>;
    VideoCaptureDeviceMFWin(ComPtr<IMFMediaSource>& source)
        :source_(source) {
        camera_stop_ = ::CreateEvent(NULL, TRUE, FALSE, "CameraStop");
        capture_error_ = ::CreateEvent(NULL, TRUE, FALSE, "CaptureError");
        capture_initialize_ = ::CreateEvent(NULL, TRUE, FALSE, "CaptureInitialize");
    }

    void SetCallback(callback_t callback) {
        std::lock_guard<std::mutex> lock(mutex_);
        callback_ = callback;
    }

    ~VideoCaptureDeviceMFWin() {
        ::CloseHandle(camera_stop_);
        ::CloseHandle(capture_error_);
        ::CloseHandle(capture_initialize_);
    }

    void OnEvent(IMFMediaEvent* media_event) {
        HRESULT hr;
        GUID capture_event_guid = GUID_NULL;

        media_event->GetStatus(&hr);
        media_event->GetExtendedType(&capture_event_guid);
        if (IsEqualGUID(capture_event_guid, MF_CAPTURE_ENGINE_ERROR) || FAILED(hr)) {
            ::SetEvent(capture_error_);
        }
        else if (IsEqualGUID(capture_event_guid, MF_CAPTURE_ENGINE_INITIALIZED)) {
            ::SetEvent(capture_initialize_);
        }
        else if (IsEqualGUID(capture_event_guid, MF_CAPTURE_ENGINE_PREVIEW_STOPPED)) {
            ::SetEvent(camera_stop_);
        }
    }

    void OnFrameDropped() {

    }


    void OnIncomingCapturedData(std::shared_ptr<ScopedBufferLock> buffer_lock) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (callback_) {
            callback_(std::move(buffer_lock));
        }
    }

    HRESULT Init() {
        HRESULT hr = S_OK;

        if (!engine_) {
            hr = CreateCaptureEngine(&engine_);
            if (FAILED(hr)) {
                assert(0);
                return hr;
            }
        }

        ComPtr<IMFAttributes> attributes;
        hr = MFCreateAttributes(&attributes, 1);
        if (FAILED(hr)) {
            assert(0);
            return hr;
        }

        hr = attributes->SetUINT32(MF_CAPTURE_ENGINE_USE_VIDEO_DEVICE_ONLY, TRUE);
        if (FAILED(hr)) {
            assert(0);
            return hr;
        }


        video_callback_ = new MFVideoCallback<VideoCaptureDeviceMFWin>(this);


        hr = engine_->Initialize(video_callback_.Get(), attributes.Get(), nullptr, source_.Get());
        if (FAILED(hr)) {
            assert(0);
            return hr;
        }

        HANDLE events[] = { capture_initialize_, capture_error_ };

        DWORD wait_result = ::WaitForMultipleObjects(2, events, FALSE, INFINITE);
        switch (wait_result) {
        case WAIT_OBJECT_0:
            break;
        case WAIT_FAILED:
            hr = HRESULT_FROM_WIN32(::GetLastError());
            assert(0);
            break;
        default:
            hr = E_UNEXPECTED;
            assert(0);
            break;
        }
        return hr;
    }

    bool GetFrameRateFromMediaType(IMFMediaType* type, float* frame_rate) {
        UINT32 numerator, denominator;
        if (FAILED(MFGetAttributeRatio(type, MF_MT_FRAME_RATE, &numerator,
            &denominator)) ||
            !denominator) {
            return false;
        }
        *frame_rate = static_cast<float>(numerator) / denominator;
        return true;
    }

    bool GetFormatFromSourceMediaType(IMFMediaType* source_media_type, bool photo, VideoCaptureFormat* format) {
        GUID major_type_guid;
        if (FAILED(source_media_type->GetGUID(MF_MT_MAJOR_TYPE, &major_type_guid)) ||
            (!IsEqualGUID(major_type_guid, MFMediaType_Image) &&
                (photo ||
                    !GetFrameRateFromMediaType(source_media_type, &format->frame_rate)))) {
            return false;
        }

        GUID sub_type_guid;
        if (FAILED(source_media_type->GetGUID(MF_MT_SUBTYPE, &sub_type_guid)) ||
            !GetFrameSizeFromMediaType(source_media_type, &format->frame_size) ||
            !GetPixelFormatFromMFSourceMediaSubtype(
                sub_type_guid, &format->pixel_format)) {
            return false;
        }

        return true;
    }


    HRESULT FillCapabilities(IMFCaptureSource* source, CapabilityList* capabilities) {
        DWORD stream_count = 0;
        HRESULT hr = source->GetDeviceStreamCount(&stream_count);
        if (FAILED(hr)) { assert(0); }

        for (DWORD stream_index = 0; stream_index < stream_count; stream_index++) {
            MF_CAPTURE_ENGINE_STREAM_CATEGORY stream_category;
            hr = source->GetDeviceStreamCategory(stream_index, &stream_category);
            if (FAILED(hr)) { assert(0); }
            if (stream_category != MF_CAPTURE_ENGINE_STREAM_CATEGORY_VIDEO_PREVIEW &&
                stream_category != MF_CAPTURE_ENGINE_STREAM_CATEGORY_VIDEO_CAPTURE) {
                continue;
            }

            DWORD media_type_index = 0;
            ComPtr<IMFMediaType> type;

            while (SUCCEEDED(source->GetAvailableDeviceMediaType(stream_index, media_type_index, &type))) {
                VideoCaptureFormat format;
                if (GetFormatFromSourceMediaType(type.Get(), false, &format))
                    capabilities->emplace_back(media_type_index, format, stream_index);
                type.Reset();
                ++media_type_index;
            }
            if (hr == MF_E_NO_MORE_TYPES) {
                hr = S_OK;
            }
            if (FAILED(hr)) {
                return hr;
            }
        }
        return hr;
    }

    HRESULT AllocateAndStart(int width, int height, UINT32 frame_rate, VideoPixelFormat& pixel_format) {
        ComPtr<IMFCaptureSource> source;
        HRESULT hr = engine_->GetSource(&source);
        if (FAILED(hr)) { return hr; }

        CapabilityList video_capabilities;
        FillCapabilities(source.Get(), &video_capabilities);

        VideoCaptureFormat requested;
        requested.frame_size.SetWidthAndHeight(width, height);
        requested.frame_rate = static_cast<float>(frame_rate);
        requested.pixel_format = pixel_format;

        const CapabilityWin best_match_video_capability =
            GetBestMatchedCapability(requested, video_capabilities);
        if (best_match_video_capability.supported_format.frame_size.width() != width ||
            best_match_video_capability.supported_format.frame_size.height() != height) {
            return -1;
        }

        pixel_format = best_match_video_capability.supported_format.pixel_format;

        ComPtr<IMFMediaType> source_video_media_type;
        hr = source->GetAvailableDeviceMediaType(best_match_video_capability.stream_index,
            best_match_video_capability.media_type_index, &source_video_media_type);
        if (FAILED(hr)) { return hr; }

        hr = source->SetCurrentDeviceMediaType(
            best_match_video_capability.stream_index, source_video_media_type.Get());
        if (FAILED(hr)) { return hr; }

        ComPtr<IMFCaptureSink> sink;
        hr = engine_->GetSink(MF_CAPTURE_ENGINE_SINK_TYPE_PREVIEW, &sink);
        if (FAILED(hr)) { return hr; }

        ComPtr<IMFCapturePreviewSink> preview_sink;
        hr = sink->QueryInterface(IID_PPV_ARGS(&preview_sink));
        if (FAILED(hr)) { return hr; }

        hr = preview_sink->RemoveAllStreams();
        if (FAILED(hr)) { return hr; }

        ComPtr<IMFMediaType> sink_video_media_type;
        hr = MFCreateMediaType(&sink_video_media_type);
        if (FAILED(hr)) { return hr; }

        hr = ConvertToVideoSinkMediaType(source_video_media_type.Get(), sink_video_media_type.Get(), frame_rate);
        if (FAILED(hr)) { return hr; }

        DWORD dw_sink_stream_index = 0;
        hr = preview_sink->AddStream(best_match_video_capability.stream_index,
            sink_video_media_type.Get(), nullptr,
            &dw_sink_stream_index);
        if (FAILED(hr)) { return hr; }

        hr = preview_sink->SetSampleCallback(dw_sink_stream_index,
            video_callback_.Get());
        if (FAILED(hr)) { return hr; }

        // Note, that it is not sufficient to wait for
        // MF_CAPTURE_ENGINE_PREVIEW_STARTED as an indicator that starting capture has
        // succeeded. If the capture device is already in use by a different
        // application, MediaFoundation will still emit
        // MF_CAPTURE_ENGINE_PREVIEW_STARTED, and only after that raise an error
        // event. For the lack of any other events indicating success, we have to wait
        // for the first video frame to arrive before sending our |OnStarted| event to
        // |client_|.
        hr = engine_->StartPreview();
        if (FAILED(hr)) { return hr; }

        return S_OK;
    }

    HRESULT Stop() {
        HRESULT hr = engine_->StopPreview();
        if (FAILED(hr)) {
            return hr;
        }

        HANDLE events[] = { camera_stop_, capture_error_ };

        DWORD wait_result = ::WaitForMultipleObjects(2, events, FALSE, INFINITE);
        if(wait_result == WAIT_OBJECT_0) {
            return S_OK;
        }
        return E_FAIL;
    }

private:
    HANDLE camera_stop_;
    HANDLE capture_error_;
    HANDLE capture_initialize_;
    ComPtr<MFVideoCallback<VideoCaptureDeviceMFWin>> video_callback_;
    ComPtr<IMFMediaSource> source_;
    ComPtr<IMFCaptureEngine> engine_;
    std::mutex mutex_;
    callback_t callback_ = nullptr;
};

// clang-format on

#endif