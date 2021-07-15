#pragma once

#include <NGIAgoraExtensionProvider.h>
#include <AgoraRefCountedObject.h>
#include "BanubaVideoProcessor.h"

namespace agora::extension {

    class BanubaExtensionProvider : public agora::rtc::IExtensionProvider {

    public:
        static void create(const std::vector<std::string> &path_to_resources,
                           const std::string &client_token,
                           int32_t width,
                           int32_t height) {
            if (s_instance == nullptr) {
                s_instance = new agora::RefCountedObject<BanubaExtensionProvider>(
                        path_to_resources,
                        client_token,
                        width,
                        height);
            }
        }

        static BanubaExtensionProvider *getInstance() {
            return s_instance;
        };

        static void release();

        BanubaExtensionProvider(const std::vector<std::string> &path_to_resources,
                                const std::string &client_token,
                                int32_t width,
                                int32_t height);

        PROVIDER_TYPE getProviderType() override;

        virtual void setExtensionControl(rtc::IExtensionControl *control) override;

        virtual agora_refptr<rtc::IAudioFilter> createAudioFilter() override;

        virtual agora_refptr<rtc::IVideoFilter> createVideoFilter() override;

        virtual agora_refptr<rtc::IVideoSinkBase> createVideoSink() override;

        int setExtensionVendor(const std::string &vendor);

    private:
        static BanubaExtensionProvider *s_instance;
        agora_refptr<BanubaVideoProcessor> m_video_processor;

    };
}