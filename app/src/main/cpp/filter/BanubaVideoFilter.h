#pragma once

#include <NGIAgoraMediaNode.h>
#include <AgoraRefCountedObject.h>
#include <AgoraRefPtr.h>
#include "BanubaVideoProcessor.h"

namespace agora::extension {

    class BanubaVideoFilter : public agora::rtc::IVideoFilter {

    public:
        BanubaVideoFilter(agora_refptr<BanubaVideoProcessor> videoProcessor);

        ~BanubaVideoFilter();

        bool adaptVideoFrame(
                const agora::media::base::VideoFrame &capturedFrame,
                agora::media::base::VideoFrame &adaptedFrame
        ) override;

        void setEnabled(bool enable) override;

        bool isEnabled() override;

        size_t setProperty(const char *key, const void *buf, size_t buf_size) override;

        size_t getProperty(const char *key, void *buf, size_t buf_size) override;

    private:
        agora_refptr<BanubaVideoProcessor> m_video_processor;
    };
}