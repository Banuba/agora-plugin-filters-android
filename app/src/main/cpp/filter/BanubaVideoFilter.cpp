#include "BanubaVideoFilter.h"
#include <sstream>
#include <utility>

namespace agora::extension {

    BanubaVideoFilter::BanubaVideoFilter(agora_refptr<BanubaVideoProcessor> videoProcessor) {
        m_video_processor = std::move(videoProcessor);
    }

    BanubaVideoFilter::~BanubaVideoFilter() = default;

    int BanubaVideoFilter::start(agora_refptr<Control> control) {
        if (control) {
            m_video_processor->set_extension_control(control);
        }
        return 0;
    }

    int BanubaVideoFilter::stop() {
        return 0;
    }

    void BanubaVideoFilter::getVideoFormatWanted(rtc::VideoFrameData::Type &type,
                                                 rtc::RawPixelBuffer::Format &format) {
        type = rtc::VideoFrameData::Type::kRawPixels;
        format = rtc::RawPixelBuffer::Format::kNV12;
    }

    void BanubaVideoFilter::getProcessMode(rtc::IExtensionVideoFilter::ProcessMode &mode,
                                           bool &independent_thread) {
        mode = rtc::IExtensionVideoFilter::ProcessMode::kSync;
        independent_thread = false;
    }

    rtc::IExtensionVideoFilter::ProcessResult BanubaVideoFilter::adaptVideoFrame(
            agora_refptr<rtc::IVideoFrame> in,
            agora_refptr<rtc::IVideoFrame> &out
    ) {
        agora::rtc::VideoFrameData captured_frame;
        in->getVideoFrameData(captured_frame);
        m_video_processor->process_frame(captured_frame);
        out = in;
        return rtc::IExtensionVideoFilter::ProcessResult::kSuccess;
    }

    int BanubaVideoFilter::setProperty(const char *key, const void *buf,
                                       size_t buf_size) {
        std::string stringParameter((char *) buf);
        m_video_processor->set_parameter(key, stringParameter);
        return 0;
    }

    int BanubaVideoFilter::getProperty(const char *key, void *buf, size_t buf_size) {
        return -1;
    }

    void BanubaVideoFilter::setEnabled(bool enable) {
    }

    bool BanubaVideoFilter::isEnabled() {
        return true;
    }
}
