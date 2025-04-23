Examples for [Banuba SDK on Android](https://docs.banuba.com/far-sdk/tutorials/development/basic_integration?platform=android) and [Agora.io](https://www.agora.io/en/) SDK integration via Agora Plugin Filters to enhance video calls with real-time face filters and virtual backgrounds.

# Getting Started

## Prerequisites

1. Visit [agora.io](https://www.agora.io/) to sign up and get the app id, client token and channel id. Please consult with [Agora documentation about account management](https://docs.agora.io/en/voice-calling/reference/manage-agora-account) to find out exactly how mentioned IDs are created.

2. Activate the [Banuba Face AR SDK extension](https://console.agora.io/marketplace/extension/introduce?serviceName=banuba) to get the app key and app secret. Please check out the [extension integration documentation](https://docs.agora.io/en/video-calling/develop/use-an-extension?platform=android) if you have any questions regarding this step.

## Dependencies

|                        | Version |                    Description                    | 
|------------------------|:-------:|:-------------------------------------------------:|
| io.agora.rtc:full-sdk  |  4.3.2  |               Agora RTC dependency                |
| Banuba SDK             | 1.15.0  | Banuba Face AR dependency for applying AR filters |
| banuba-agora-extension |  1.5.0  |            Banuba Extension for Agora             |


## Installation

1. Run the following command in the Terminal to clone the project
```sh
git clone https://github.com/Banuba/agora-plugin-filters-android.git
```
2. Add Banuba dependencies.
Add repository to [build.gradle](build.gradle#L17) to get Banuba dependencies
```groovy
maven {
  name "GitHubPackages"
  url "https://maven.pkg.github.com/sdk-banuba/banuba-sdk-android"
  credentials {
    username = "sdk-banuba"
    password = "\u0067\u0068\u0070\u005f\u004a\u0067\u0044\u0052\u0079\u0049\u0032\u006d\u0032\u004e\u0055\u0059\u006f\u0033\u0033\u006b\u0072\u0034\u0049\u0069\u0039\u0049\u006f\u006d\u0077\u0034\u0052\u0057\u0043\u0064\u0030\u0052\u0078\u006d\u0045\u0069"
  }
}
```
Add required dependencies to [app/build.gradle](app/build.gradle#L39).
```groovy
dependencies {
    // Banuba Face AR dependency for playing AR effects
    implementation "com.banuba.sdk:banuba_sdk:1.17.1"

    // Banuba extension for Agora
    implementation 'com.banuba.sdk.android:agora-extension:1.5.1'
    ...
}
```

3. Add Agora dependency `implementation 'io.agora.rtc:full-sdk:x.x.x'` in [app/build.gradle](app/build.gradle#L47)

4. Add your [keys](app/src/main/java/com/banuba/sdk/agorapluginexample/Keys.kt) to run the sample.
```kotlin
// Use Agora appId key created in Agora console
const val AGORA_APP_ID = ...

// Use Agora channel created in console
const val AGORA_CHANNEL = ...

// Use token created in Agora console with Agora by channel
// Important! This token can be omitted if your Agora project is in testing mode
const val AGORA_TOKEN = ""

// Use Banuba license token
const val BANUBA_LICENSE_TOKEN: String = ...
```

5. The sample includes a few basic AR effects, however you can [download](https://docs.banuba.com/face-ar-sdk-v1/overview/demo_face_filters) more. All effects must be added into [effects](app/src/main/assets/effects) folder in Android assets. 
Please modify the list of [available effects](app/src/main/java/com/banuba/sdk/agorapluginexample/MainActivity.kt#L38) in the sample.

## Integration
The extension includes main `BanubaExtensionManager` class for interacting with AR effects.  
Basically, AR effects should be placed to [assets/effects](app/src/main/assets/effects) folder.  
Additionally, you can store AR effects on device internal storage and use `File` to load effect.

### Enable extension
First, add Banuba extension `BanubaExtensionManager.BANUBA_PLUGIN_NAME` while initializing instance `RtcEngineConfig`
```diff
val rtcEngineConfig = RtcEngineConfig()
+rtcEngineConfig.addExtension(BANUBA_PLUGIN_NAME)
```
Next, you need to enable the extension in `onCreate` method by providing specific provider and extension names.
```diff
override fun onCreate(savedInstanceState: Bundle?) {
   super.onCreate(savedInstanceState)
   ...
+  agoraRtc.enableExtension(
+     BANUBA_PROVIDER_NAME,
+     BANUBA_EXTENSION_NAME,
+     true,
+     Constants.MediaSourceType.PRIMARY_CAMERA_SOURCE
+  )
}
```
and disable the extension and Agora in `onDestroy` method
```diff
override fun onDestroy() {
   super.onDestroy()

   BanubaExtensionManager.destroy()

+  agoraRtc.enableExtension(
+     BANUBA_PROVIDER_NAME,
+     BANUBA_EXTENSION_NAME,
+     false,
+     Constants.MediaSourceType.PRIMARY_CAMERA_SOURCE
+  )
   
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

+BanubaExtensionManager.initialize(
+   applicationContext,
+   BANUBA_LICENSE_TOKEN,
+   agoraRtc
+)
```

#### Load effects from assets 

Use `BanubaExtensionManager.loadEffect` method for applying AR effects stored in Android `assets/effects` folder.

#### Load effects from path

Use `agoraRtc.setExtensionProperty` method for applying AR effects stored on device internal storage.
Use empty string `""` to cancel an effect.

```kotlin
agoraRtc.setExtensionProperty(
   BANUBA_PROVIDER_NAME,  // "Banuba"
   BANUBA_EXTENSION_NAME, // "BanubaFilter"
   "load_effect",
   path_to_effect
)
```

Finally, it is highly recommended to bind applying AR effects to Android lifecycle events by overriding [onResume](app/src/main/java/com/banuba/sdk/agorapluginexample/MainActivity.kt#L177) and [onPause](app/src/main/java/com/banuba/sdk/agorapluginexample/MainActivity.kt#L185) methods
otherwise current effect might be applying even though the app is in the background and the user is not interacting with it.
```diff
     override fun onResume() {
        super.onResume()
        // Required to resume current Banuba Face AR effect
+       BanubaExtensionManager.resume()
     }

    override fun onPause() {
        super.onPause()

        // Required to pause current Banuba Face AR effect i.e. some AR effects might play audio or
        // continue processing something in background. ".pause" method helps to stop processing.
+       BanubaExtensionManager.pause()
    }
```

### Interact with Effect API
Use ```BanubaExtensionManager.evalJs()``` method and pass String commands i.e. "Background = require('bnb_js/background'); Background.blur(0.6);"
to interact with the effect API.
Please visit [Effect API](https://docs.banuba.com/face-ar-sdk-v1/effect_api/getting_started) guidelines to know more this feature.
