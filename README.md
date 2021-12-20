Examples for [Banuba SDK on Android](https://docs.banuba.com/face-ar-sdk-v1/android/android_overview) and [Agora.io](https://www.agora.io/en/) SDK integration via Agora Plugin Filters to enhance video calls with real-time face filters and virtual backgrounds.

# Getting Started

1. Get
[(a) The latest Banuba SDK archive and (b) BanubaFiltersAgoraExtension for Android](https://f.hubspotusercontent10.net/hubfs/4992313/Agora_Banuba_Extension/AgoraBanubaExtension_Android_v1.x.x.zip),
(c) [Banuba trial client token](https://www.banuba.com/facear-sdk/face-filters).
To receive full commercial licence from Banuba - please fill in our form on [form on banuba.com](https://www.banuba.com/face-filters-sdk) website, or contact us via [info@banuba.com](mailto:info@banuba.com).
2. Copy `aar` files from the Banuba SDK archive into `libs` dir:
    `banuba_effect_player-release.aar` => `app/libs/`
3. Get the latest Banuba Agora plugin library for Android.
4. Copy `aar` files from the Banuba Agora plugin library into `libs` dir:
    `banuba-agora-plugin-release.aar` => `app/libs/`
5. Get [The latest Agora Video SDK archive for android](https://docs.agora.io/en/extension_customer/Banuba_downloads).
6. Copy `jar` files from the Agora Video SDK archive into `libs` dir:
    `agora-rtc-sdk.jar` => `app/libs/`
7. Copy architecture folders from the Agora Video SDK archive into `jniLibs` dir:
    `arm64-v8a`, `armeabi-v7a` and `x86_64` => `app/src/main/jniLibs/`
8. Copy and Paste your banuba client token into appropriate section of `app/src/main/java/com/banuba/sdk/agorapluginexample/ClientToken.kt` with “ ” symbols. For example: BANUBA_CLIENT_TOKEN = “place_your_token_here”
9. Visit agora.io to sign up and get token, app ID and channel ID.
10. Copy and Paste your agora token, app ID and chanel ID into appropriate section of `app/src/main/java/com/banuba/sdk/agorapluginexample/ClientToken.kt` with “ ” symbols. For example: AGORA_APP_ID = “place_your_token_here”
11. Open the project in Android Studio and run the necessary target using the usual steps.

# How to use `BanubaFiltersAgoraExtension`

Add following imports:
```kotlin
import com.banuba.agora.plugin.BanubaEffectsLoader
import com.banuba.agora.plugin.BanubaExtensionManager
import com.banuba.agora.plugin.BanubaResourceManager
import com.banuba.agora.plugin.model.ArEffect
```

Add extension to RtcEngineConfig:
```kotlin
RtcEngineConfig().apply {
 ...
 System.loadLibrary("banuba")
 addExtension(BanubaExtensionManager.EXTENSION_NAME)
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
5. MorphTeenegers
6. Nerd
7. Popeye
8. SnapBubblesGlasses
9. Space
10. StarGlow_low
11. TitanicJack
