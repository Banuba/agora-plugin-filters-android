package com.banuba.sdk.agorapluginexample

import androidx.annotation.Keep

@Keep
object ExtensionManager {

    const val VENDOR_NAME_VIDEO = "BanubaFilter"
    const val KEY_LOAD_EFFECT = "load_effect"

    init {
        System.loadLibrary("native-lib")
    }

    external fun nativeGetExtensionProvider(
        vendor: String,
        resourcesPath: String,
        token: String,
        width: Int,
        height: Int
    ): Long
}