#include <filter/BanubaExtensionProvider.h>
#include <AgoraExtensionProviderEntry.h>

REGISTER_AGORA_EXTENSION_PROVIDER(Banuba, agora::extension::BanubaExtensionProvider, agora::rtc::IExtensionVideoFilter)
