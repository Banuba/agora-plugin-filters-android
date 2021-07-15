#include <string>
#include "BanubaExtensionProvider.h"
#include "BanubaVideoFilter.h"

namespace agora::extension {

    BanubaExtensionProvider *BanubaExtensionProvider::s_instance = nullptr;

    BanubaExtensionProvider::BanubaExtensionProvider(
            const std::vector<std::string> &path_to_resources,
            const std::string &client_token,
            int32_t width,
            int32_t height) {
        m_video_processor = new agora::RefCountedObject<BanubaVideoProcessor>(
                path_to_resources,
                client_token,
                width,
                height
        );
    }

    void BanubaExtensionProvider::release() {
        if (s_instance) {
            delete s_instance;
            s_instance = nullptr;
        }
    }

    agora_refptr<agora::rtc::IVideoFilter> BanubaExtensionProvider::createVideoFilter() {
        auto videoFilter = new agora::RefCountedObject<agora::extension::BanubaVideoFilter>(
                m_video_processor
        );
        return videoFilter;
    }

    agora_refptr<agora::rtc::IAudioFilter> BanubaExtensionProvider::createAudioFilter() {
        return nullptr;
    }

    agora_refptr<agora::rtc::IVideoSinkBase> BanubaExtensionProvider::createVideoSink() {
        return nullptr;
    }

    BanubaExtensionProvider::PROVIDER_TYPE BanubaExtensionProvider::getProviderType() {
        return agora::rtc::IExtensionProvider::LOCAL_VIDEO_FILTER;
    }

    void BanubaExtensionProvider::setExtensionControl(rtc::IExtensionControl *control) {
        m_video_processor->set_extension_control(control);
    }

    int BanubaExtensionProvider::setExtensionVendor(const std::string &vendor) {
        m_video_processor->set_extension_vendor(vendor.c_str());
        return 0;
    }
}
