#include <string>
#include "BanubaExtensionProvider.h"
#include "BanubaVideoFilter.h"

namespace agora::extension {

    BanubaExtensionProvider::BanubaExtensionProvider() {
        m_video_processor = new agora::RefCountedObject<BanubaVideoProcessor>();
    }

    BanubaExtensionProvider::~BanubaExtensionProvider() {
        m_video_processor.reset();
    }

    agora_refptr<agora::rtc::IExtensionVideoFilter> BanubaExtensionProvider::createVideoFilter(
            const char *name
    ) {
        auto videoFilter = new agora::RefCountedObject<agora::extension::BanubaVideoFilter>(
                m_video_processor
        );
        return videoFilter;
    }

    agora_refptr<agora::rtc::IAudioFilter> BanubaExtensionProvider::createAudioFilter(
            const char *name
    ) {
        return nullptr;
    }

    agora_refptr<agora::rtc::IVideoSinkBase> BanubaExtensionProvider::createVideoSink(
            const char *name
    ) {
        return nullptr;
    }

    void BanubaExtensionProvider::setExtensionControl(rtc::IExtensionControl *control) {
    }

    void BanubaExtensionProvider::enumerateExtensions(
            rtc::IExtensionProvider::ExtensionMetaInfo *extension_list,
            int &extension_count
    ) {
        extension_count = 1;
        extension_list[0] = ExtensionMetaInfo{
                .type = EXTENSION_TYPE::VIDEO_PRE_PROCESSING_FILTER,
                .extension_name = "BanubaFilter"
        };
    }
}
