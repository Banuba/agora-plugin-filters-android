#pragma once

#include <NGIAgoraExtensionProvider.h>
#include "BanubaVideoProcessor.h"

namespace agora::extension {

    class BanubaExtensionProvider : public agora::rtc::IExtensionProvider {

    public:
        BanubaExtensionProvider();
        ~BanubaExtensionProvider();

        void enumerateExtensions(ExtensionMetaInfo *extension_list, int &extension_count) override;
        void setExtensionControl(rtc::IExtensionControl *control) override;

        agora_refptr<rtc::IExtensionVideoFilter> createVideoFilter(const char* name) override;
        agora_refptr<rtc::IAudioFilter> createAudioFilter(const char *name) override;
        agora_refptr<rtc::IVideoSinkBase> createVideoSink(const char *name) override;

    private:
        agora_refptr<BanubaVideoProcessor> m_video_processor;
    };
}