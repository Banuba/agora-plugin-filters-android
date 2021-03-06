Examples for [Banuba SDK on Android](https://docs.banuba.com/face-ar-sdk-v1/android/android_overview) and [Agora.io](https://www.agora.io/en/) SDK integration via Agora Plugin Filters to enhance video calls with real-time face filters and virtual backgrounds.

> **Important**
>
> [master](../../tree/master) branch is always compatible with latest SDK version. Please use [legacy](../../tree/legacy) branch for SDK version 0.x (e.g. v0.38.x).


# Getting Started

0. Make sure that you have Android **NDK** and **CMake** are installed! Project contains C++ sources, so this is required! Recommended NDK versions is 21.1.6352462, required CMake version is 3.9, 3.14 or above is recommended (Android SDK manager provides 3.18.1, it should be installed).
1. Clone the project to your computer:
    ```sh
        git clone --recursive https://github.com/Banuba/agora-plugin-filters-android.git
    ```
2. Get the latest Banuba SDK archive, [Banuba trial client token](https://www.banuba.com/facear-sdk/face-filters).
To receive full commercial license from Banuba - please fill in our form on [form on banuba.com](https://www.banuba.com/face-filters-sdk) website, or contact us via [info@banuba.com](mailto:info@banuba.com).
3. Copy `.aar` file from the Banuba SDK archive into `libs` dir:
   >`banuba_effect_player-release.aar` => `libs/`
4. Copy `include/bnb` directory into `libs/bnb_sdk/`:
   >`include/bnb` => `libs/bnb_sdk/bnb`
5. Download and copy folders of the needed effects into `app/src/main/assets/effects` directory. The effects can be downloaded from [here](https://docs.banuba.com/face-ar-sdk-v1/overview/demo_face_filters).
6. Get the Agora Video SDK archive for android (v4.0.0-beta.2) from [here](https://docs.agora.io/en/extension_customer/downloads?platform=Android).
7. Copy `jar` files from the Agora Video SDK archive into `libs` dir:
   >`agora-rtc-sdk.jar` => `libs/`
8. Copy architecture folders from the Agora Video SDK archive into `jniLibs` dir:
   >`arm64-v8a`, `armeabi-v7a` and `x86_64` => `libs/jniLibs/`
9. Copy Agora SDK headers (`*.h` files) into `libs/agora_rtc_sdk/include`:
   >`low_level_api/include` => `libs/agora_rtc_sdk/include`
10. Copy and Paste your Banuba client token into appropriate section of `app/src/main/java/com/banuba/sdk/agorapluginexample/ClientToken.kt` with " " symbols. For example: BANUBA_CLIENT_TOKEN = "place_your_token_here"
11. Visit [agora.io](https://console.agora.io) to sign up and get token, app ID and channel ID.
12. Copy and Paste your Agora token, app ID and channel ID into appropriate section of `app/src/main/java/com/banuba/sdk/agorapluginexample/ClientToken.kt` with " " symbols. For example: AGORA_APP_ID = "place_your_token_here"
13. Open the project in Android Studio and run the necessary target using the usual steps.

# How to use `BanubaFiltersAgoraExtension`

Add following imports:

```kotlin
import com.banuba.agora.plugin.BanubaEffectsLoader
import com.banuba.agora.plugin.BanubaExtensionManager
import com.banuba.agora.plugin.BanubaResourceManager
import com.banuba.agora.plugin.model.ArEffect
import com.banuba.sdk.utils.ContextProvider
```

Add extension to RtcEngineConfig:

```kotlin
RtcEngineConfig().apply {
 ...
 System.loadLibrary("banuba")
 addExtension(BanubaExtensionManager.EXTENSION_NAME)
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
  BanubaExtensionManager.VENDOR_NAME,
  BanubaExtensionManager.VIDEO_FILTER_NAME,
  true
)
```

```kotlin
private fun initBanubaPlugin() {
  agoraRtc.setExtensionProperty(
    BanubaExtensionManager.VENDOR_NAME,
    BanubaExtensionManager.VIDEO_FILTER_NAME,
    BanubaExtensionManager.KEY_SET_RESOURCES_PATH,
    banubaResourceManager.resourcesPath
  )
  agoraRtc.setExtensionProperty(
    BanubaExtensionManager.VENDOR_NAME,
    BanubaExtensionManager.VIDEO_FILTER_NAME,
    BanubaExtensionManager.KEY_SET_EFFECTS_PATH,
    banubaResourceManager.effectsPath
  )
  agoraRtc.setExtensionProperty(
    BanubaExtensionManager.VENDOR_NAME,
    BanubaExtensionManager.VIDEO_FILTER_NAME,
    BanubaExtensionManager.KEY_SET_TOKEN,
    BANUBA_CLIENT_TOKEN
  )
}
```

To enable/disable effects use the following code:

```kotlin
private val onEffectPrepared = object : BanubaResourceManager.EffectPreparedCallback {
  override fun onPrepared(effectName: String) {
    sendEffectToFilter(effectName)
  }
}

private fun sendEffectToFilter(effect: String) {
  agoraRtc.setExtensionProperty(
    BanubaExtensionManager.VENDOR_NAME,
    BanubaExtensionManager.VIDEO_FILTER_NAME,
    BanubaExtensionManager.KEY_LOAD_EFFECT,
    effect
  )
}
```

```kotlin
banubaResourceManager.prepareEffect(Effect name, onEffectPrepared)
```

[Check out example](app/src/main/java/com/banuba/sdk/agorapluginexample/MainActivity.kt)

# Effects managing

To retrieve current effects use the following code:

```kotlin
val effects: List<ArEffect> = BanubaEffectsLoader(this).loadEffects()
```

ArEffect contains following information:\
`name: String` - pass to `banubaResourceManager.prepareEffect(Effect name, onEffectPrepared)`. Also can be used to display label on the UI\
`preview: Bitmap` - can be used as preview image

To modify effects, add or remove effect folder in `app/src/main/assets/effects` directory.
By default sample contains the following effects:

1. ElvisUnleashed
2. EnglandEightPieceCap
3. FashionHistory1940_male
4. MorphingHatBow
5. Nerd
6. SnapBubblesGlasses
7. Space
8. StarGlow
9. TitanicJack
