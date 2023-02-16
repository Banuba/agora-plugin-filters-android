Examples for [Banuba SDK on Android](https://docs.banuba.com/face-ar-sdk-v1/android/android_overview) and [Agora.io](https://www.agora.io/en/) SDK integration via Agora Plugin Filters to enhance video calls with real-time face filters and virtual backgrounds.

# Getting Started

0. Make sure that you have Android **NDK** and **CMake** are installed! Project contains C++ sources, so this is required! Recommended NDK versions is 21.1.6352462, required CMake version is 3.9, 3.14 or above is recommended (Android SDK manager provides 3.18.1, it should be installed).
1. Clone the project to your computer:
    ```sh
        git clone --recursive https://github.com/Banuba/agora-plugin-filters-android.git
    ```
    > Important: The example uses submodules. So when cloning, don't forget to update the submodules to the latest version.
    > `git submodule --update --recursive --init` will help you with this.
2. Get the latest Banuba SDK archive, [Banuba trial client token](https://www.banuba.com/facear-sdk/face-filters).
To receive full commercial license from Banuba - please fill in our form on [form on banuba.com](https://www.banuba.com/face-filters-sdk) website, or contact us via [info@banuba.com](mailto:info@banuba.com).
3. Copy `.aar` file from the Banuba SDK archive into `libs` dir:
   >`banuba_effect_player-release.aar` => [`libs/`](./libs)
4. Copy `include/bnb` directory into [`libs/bnb_sdk/`](./libs/bnb_sdk):
   >`include/bnb` => [`libs/bnb_sdk/`](./libs/bnb_sdk)
5. Download the effects you need from [HERE](https://docs.banuba.com/face-ar-sdk-v1/overview/demo_face_filters). Unzip the downloaded `.zip` files with effects to a folder [`app/src/main/assets/effects`](./app/src/main/assets/effects). Each unpacked effect should be put into a separate folder. The folder name will be the effect name for loading.
6. Get the Agora Video SDK archive for android (v4.0.1) from [HERE](https://download.agora.io/sdk/release/Agora_Native_SDK_for_Android_v4.0.1_FULL.zip).
7. Copy `.jar` files from the Agora Video SDK archive into [`libs`](./libs) dir:
   >`agora-rtc-sdk.jar` => [`libs/`](./libs)
8. Copy architecture folders from the Agora Video SDK archive into [`jniLibs`](./libs/jniLibs) dir and merge them with existing folders:
   >`arm64-v8a`, `armeabi-v7a` and `x86_64` => [`libs/jniLibs/`](./libs/jniLibs)
9. Copy the `low_level_api/include` directory from Agora SDK into [`libs/agora_rtc_sdk/`](./libs/agora_rtc_sdk/):
   >`low_level_api/include` => [`libs/agora_rtc_sdk/`](./libs/agora_rtc_sdk/)
10. As a result of the previous steps, the [`libs`](./libs) folder will contain:
```
libs/agora_rtc_sdk/include/             - the 'include' folder can be found in the archive: (Agora RTC archive)/rtc/sdk/low_level_api/
libs/bnb_sdk/bnb/                       - the 'bnb' folder can be found in the archive: (Banuba SDK archive)/BNBEffectPlayer/include/
libs/jniLibs/arm64-v8a/                 - the 'arm64-v8a' folder can be found in the archive: (Agora RTC archive)/rtc/sdk/
libs/jniLibs/armeabi-v7a/               - the 'armeabi-v7a' folder can be found in the archive: (Agora RTC archive)/rtc/sdk/
libs/jniLibs/x86_64/                    - the 'x86_64' folder can be found in the archive: (Agora RTC archive)/rtc/sdk/
libs/agora-rtc-sdk.jar                  - the 'agora-rtc-sdk.jar' file can be found in the archive: (Agora RTC archive)/rtc/sdk/
libs/banuba_effect_player-release.aar   - the 'banuba_effect_player-release.aar' file can be found in the archive: (Banuba SDK archive)/BNBEffectPlayer/
```
11. Copy and Paste your Banuba client token into appropriate section of [`ClientToken.kt`](./app/src/main/java/com/banuba/sdk/agorapluginexample/ClientToken.kt) with " " symbols.
12. Visit [agora.io](https://console.agora.io) to sign up and get [Agora token, Agora app ID and Agora channel ID](https://docs.agora.io/en/Agora%20Platform/channel_key?platform=Android). You can read more about how you can get an Agora token at [THIS LINK](https://docs.agora.io/en/Agora%20Platform/channel_key?platform=Android).
13. Copy and Paste your Agora token, Agora app ID and Agora channel ID into appropriate section of [`ClientToken.kt`](./app/src/main/java/com/banuba/sdk/agorapluginexample/ClientToken.kt) with " " symbols.
14. As a result of the two previous steps, in the file [`ClientToken.kt`](./app/src/main/java/com/banuba/sdk/agorapluginexample/ClientToken.kt) you should have the following:
```kotlin
package com.banuba.sdk.agorapluginexample

const val AGORA_APP_ID = "this_is_your_AGORA_APP_ID"
const val AGORA_CLIENT_TOKEN = "this_is_your_AGORA_CLIENT_TOKEN"
const val AGORA_CHANNEL_ID = "this_is_your_AGORA_CHANNEL_ID"

const val BANUBA_CLIENT_TOKEN: String = "this_is_your_BANUBA_CLIENT_TOKEN"
```
15. Open the project in Android Studio and run the necessary target using the usual steps.

# Effects managing

To add new effects to the example, please download effects [HERE](https://docs.banuba.com/face-ar-sdk-v1/overview/demo_face_filters) and put them in this folder [`app/src/main/assets/effects`](./app/src/main/assets/effects).
By default the sample doesn't contains effects.

To retrieve current effects use the following code:

```kotlin
val effects: List<ArEffect> = BanubaEffectsLoader(this).loadEffects()
```

ArEffect contains following information:\
`name: String` - pass to `banubaResourceManager.prepareEffect(Effect name, onEffectPrepared)`. Also can be used to display label on the UI\
`preview: Bitmap` - can be used as preview image

# How to use `BanubaFiltersAgoraExtension`

Add following imports:

```kotlin
import com.banuba.agora.plugin.BanubaEffectsLoader
import com.banuba.agora.plugin.BanubaExtensionManager
import com.banuba.agora.plugin.BanubaResourceManager
import com.banuba.agora.plugin.model.ArEffect
import com.banuba.sdk.utils.ContextProvider
```

Create the Banuba SDK extension object:

```kotlin
private val banubaExtension = BanubaExtensionManager(object : AgoraInterface {
  override fun onSetExtensionProperty(provider: String, extension: String, propertyKey: String, propertyValue: String) {
    agoraRtc.setExtensionProperty(provider, extension, propertyKey, propertyValue)
  }
})

/* This interface separates banuba SDK and Agora rtc engine from each other */
private val banubaExtensionInterface: AgoraExtension = banubaExtension
```

Add extension to RtcEngineConfig:

```kotlin
RtcEngineConfig().apply {
  ...
  System.loadLibrary(banubaExtensionInterface.getLibraryName())
  addExtension(banubaExtensionInterface.getPluginName())
  ContextProvider.setContext(mContext)
  ...
}
```

Create and initialize BanubaResourceManager:

```kotlin
private val banubaResourceManager by lazy(LazyThreadSafetyMode.NONE) {
  BanubaResourceManager(this)
}
```

```kotlin
override fun onCreate(savedInstanceState: Bundle?) {
  ...
  banubaResourceManager.prepare()
}
```

After those steps enable and initialize extension:

```kotlin
agoraRtc.enableExtension(
  banubaExtensionInterface.getProviderName(),
  banubaExtensionInterface.getExtensionName(),
  true
)
```

```kotlin
private fun initializeBanubaExtension() {
  banubaExtension.initialize(
    banubaResourceManager.resourcesPath,
    banubaResourceManager.effectsPath,
    BANUBA_CLIENT_TOKEN
  )
  banubaExtension.create()
  banubaExtension.setDeviceOrientation(
    getDeviceOrientationDegrees(this)
  )
}
```

To enable/disable effects use the following code:

```kotlin
private val onEffectPrepared = object : BanubaResourceManager.EffectPreparedCallback {
  override fun onPrepared(effectName: String) {
    banubaExtension.loadEffect(effectName)
  }
}
```

```kotlin
banubaResourceManager.prepareEffect(Effect name, onEffectPrepared)
```

[Check out example](app/src/main/java/com/banuba/sdk/agorapluginexample/MainActivity.kt)
