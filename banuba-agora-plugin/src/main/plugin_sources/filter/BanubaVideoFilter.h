#pragma once

#include <NGIAgoraMediaNode.h>
#include <AgoraRefCountedObject.h>
#include <AgoraRefPtr.h>
#include "BanubaVideoProcessor.h"

namespace agora::extension {

    class BanubaVideoFilter : public agora::rtc::IExtensionVideoFilter {

    public:
        BanubaVideoFilter(agora_refptr<BanubaVideoProcessor> videoProcessor);
        ~BanubaVideoFilter();

        int start(agora::agora_refptr<Control> control) override;
        int stop() override;

        void getVideoFormatWanted(
                rtc::VideoFrameData::Type &type,
                rtc::RawPixelBuffer::Format &format
        ) override;

        void getProcessMode(ProcessMode &mode, bool &independent_thread) override;

        ProcessResult pendVideoFrame(agora_refptr<rtc::IVideoFrame> frame) override;

        void setEnabled(bool enable) override;
        bool isEnabled() override;

        int setProperty(const char *key, const void *buf, size_t buf_size) override;
        int getProperty(const char *key, void *buf, size_t buf_size) override;

    private:
        agora_refptr<BanubaVideoProcessor> m_video_processor;
    };
}
