Examples for [Banuba SDK on Android](https://docs.banuba.com/face-ar-sdk-v1/android/android_overview) and [Agora.io](https://www.agora.io/en/) SDK integration via Agora Plugin Filters to enhance video calls with real-time face filters and virtual backgrounds.

# Getting Started

## Prerequisites

1. Visit [agora.io](https://www.agora.io/) to sign up and get the app id, client token and channel id. Please consult with [Agora documentation about account management](https://docs.agora.io/en/voice-calling/reference/manage-agora-account) to find out exactly how mentioned IDs are created.

2. Activate the [Banuba Face AR SDK extension](https://console.agora.io/marketplace/extension/introduce?serviceName=banuba) to get the app key and app secret. Please check out the [extension integration documentation](https://docs.agora.io/en/video-calling/develop/use-an-extension?platform=android) if you have any questions regarding this step.

## Dependencies

|                                      | Version |                    Description                    | 
|--------------------------------------|:-------:|:-------------------------------------------------:|
| agora-rtc-sdk.jar                            |  4.1.1  |               Agora RTC dependency                |
| banuba_effect_player-release.aar     |  1.6.0  | Banuba Face AR dependency for applying AR filters |
| banuba-agora-extension-*-release.aar |  1.1.1  |            Banuba Extension for Agora             |


## Installation

1. Run the following command in the Terminal to clone the project
```sh
git clone https://github.com/Banuba/agora-plugin-filters-android.git
```

2. Add `banuba_effect_player-release.aar`, `banuba-agora-extension-1.1.1-release.aar` files into [libs](libs) folder and 
   sync your project.
3. Add `implementation 'io.agora.rtc:full-sdk:4.1.1'` dependency in [app gradle](app/build.gradle#L45)

4. Add your [keys](app/src/main/java/com/banuba/sdk/agorapluginexample/Keys.kt) to run the sample.
```kotlin
// Use Agora appId key created in Agora console
const val AGORA_APP_ID: String = ...
// Use Agora channel created in console
const val AGORA_CHANNEL: String = ...
// Use token created in Agora console with Agora appId and channel
// Important! The token is optional if your Agora project is in testing mode
const val AGORA_TOKEN: String = ...

// Use Banuba license token
const val BANUBA_LICENSE_TOKEN: String = ...

// Banuba Ext app key
const val BANUBA_EXT_APP_KEY = ...
```

4. The sample includes a few basic AR effects and you can [download](https://docs.banuba.com/face-ar-sdk-v1/overview/demo_face_filters) more. All effects must be added into [effects](app/src/main/assets/effects) folder in Android assets. 
Please modify the list of [available effects](app/src/main/java/com/banuba/sdk/agorapluginexample/MainActivity.kt#L38) in the sample.

## Integration
The extension includes main `BanubaExtensionManager` class for interacting with AR effects.  
AR effects should be placed to [effects](app/src/main/assets/effects) folder in Android assets.

### Enable extension
First, add Banuba extension `BanubaExtensionManager.BANUBA_PLUGIN_NAME` while initializing instance `RtcEngineConfig`
```diff
val rtcEngineConfig = RtcEngineConfig()
+ rtcEngineConfig.addExtension(BANUBA_PLUGIN_NAME)
```
Next, you need to enable the extension in `onCreate` method by providing specific provider and extension names.
```diff
override fun onCreate(savedInstanceState: Bundle?) {
   super.onCreate(savedInstanceState)
   ...
+   agoraRtc.enableExtension(
      BANUBA_PROVIDER_NAME,
      BANUBA_EXTENSION_NAME,
      true,
      Constants.MediaSourceType.PRIMARY_CAMERA_SOURCE
   )
}
```
and disable the extension and Agora in `onDestroy` method
```diff
override fun onDestroy() {
   super.onDestroy()

   BanubaExtensionManager.destroy()

+   agoraRtc.enableExtension(
      BANUBA_PROVIDER_NAME,
      BANUBA_EXTENSION_NAME,
      false,
      Constants.MediaSourceType.PRIMARY_CAMERA_SOURCE
   )
   
   RtcEngine.destroy()
}
```

### Manage effects
Initialize extension using `BanubaExtensionManager.initialize` method by providing a number of required params.  
Please keep in mind that initialize should be invoked only after starting preview `agoraRtc.startPreview()`.
```diff
 // Important - extension works only after start preview
addLocalVideo()
agoraRtc.startPreview()

+ BanubaExtensionManager.initialize(
   applicationContext,
   BANUBA_LICENSE_TOKEN,
   agoraRtc,
   BANUBA_EXT_APP_KEY,
)
```

Use `BanubaExtensionManager.loadEffect` method for applying AR effects. Use empty string `""` for `effectName` to cancel an effect.  

Finally, it is highly recommended to bind applying AR effects to Android lifecycle events by overriding [onResume](app/src/main/java/com/banuba/sdk/agorapluginexample/MainActivity.kt#L177) and [onPause](app/src/main/java/com/banuba/sdk/agorapluginexample/MainActivity.kt#L185) methods
otherwise current effect might be applying even though the app is in the background and the user is not interacting with it.
```diff
     override fun onResume() {
        super.onResume()
        // Required to resume current Banuba Face AR effect
+        BanubaExtensionManager.resume()
     }

    override fun onPause() {
        super.onPause()

        // Required to pause current Banuba Face AR effect i.e. some AR effects might play audio or
        // continue processing something in background. ".pause" method helps to stop processing.
+        BanubaExtensionManager.pause()
    }
```
