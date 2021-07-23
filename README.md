Examples for [Banuba SDK on Android](https://docs.banuba.com/face-ar-sdk/android/android_overview) and [Agora.io](https://www.agora.io/en/) SDK integration via Agora Plugin Filters to enhance video calls with real-time face filters and virtual backgrounds.

# Getting Started

1. Get the latest Banuba SDK archive for Android and the client token. Please fill in our form on [form on banuba.com](https://www.banuba.com/face-filters-sdk) website, or contact us via [info@banuba.com](mailto:info@banuba.com).
2. Copy `aar` files from the Banuba SDK archive into `libs` dir:
    `BNBEffectPlayer/banuba_effect_player-release.aar` => `app/libs/`
    `BNBEffectPlayer/include/bnb` => `app/src/main/cpp/bnb_sdk/`
3. Copy and Paste your banuba client token into appropriate section of `app/src/main/java/com/banuba/sdk/agorapluginexample/ClientToken.kt` with “ ” symbols. For example: BANUBA_CLIENT_TOKEN = “place_your_token_here”
4. Visit agora.io to sign up and get token, app and channel ID
5. Copy and Paste your agora token, app and chanel ID into appropriate section of `app/src/main/java/com/banuba/sdk/agorapluginexample/ClientToken.kt` with “ ” symbols. For example: AGORA_APP_ID = “place_your_token_here”
6. Open the project in Android Studio and run the necessary target using the usual steps.