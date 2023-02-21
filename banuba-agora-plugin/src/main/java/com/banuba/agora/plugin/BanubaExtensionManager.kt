package com.banuba.agora.plugin

import android.util.Log

class BanubaExtensionManager(val callback: AgoraExtensionCallback) {

    interface AgoraExtensionCallback {
        fun onExtensionPropertySet(key: String, value: String)
    }

    companion object {
        private const val NATIVE_LIBRARY_NAME = "banuba"
        const val PLUGIN_NAME = "banuba-plugin" // CMake target
        const val PROVIDER_NAME = "Banuba"
        const val EXTENSION_NAME = "BanubaFilter"

        // init/destroy. Remove create
        private const val KEY_INITIALIZE = "initialize"
        private const val KEY_RESOURCES_PATH = "set_resources_path"
        private const val KEY_EFFECTS_PATH = "set_effects_path"
        private const val KEY_CLIENT_TOKEN = "set_client_token"
        private const val KEY_RESUME = "resume"
        private const val KEY_LOAD_EFFECT = "load_effect"
        private const val KEY_UNLOAD_EFFECT = "unload_effect"
        private const val KEY_PAUSE = "pause"
        private const val KEY_DESTROY = "destroy"

        private const val EMPTY_PARAMETER = "1" // empty parameter to make Agora process value
    }

    init {
        try {
            System.loadLibrary(NATIVE_LIBRARY_NAME)
        } catch (e: Throwable) {
            Log.e("Banuba", "Cannot load Banuba native library")
        }
    }

    /* Initializes Banuba extension with params */
    fun initialize(
        resourcesPath: String,
        effectsPath: String,
        clientToken: String
    ) {
        dispatchProperty(KEY_RESOURCES_PATH, resourcesPath);
        dispatchProperty(KEY_EFFECTS_PATH, effectsPath);

        // Piotr, remove it after refactoring and pass token to KEY_INITIALIZE
        dispatchProperty(KEY_CLIENT_TOKEN, clientToken);

        dispatchProperty(KEY_INITIALIZE, EMPTY_PARAMETER)

        // Piotr, remove it after refactoring
        dispatchProperty("create", EMPTY_PARAMETER);
    }

    // Destroys OEP Surface
    fun destroy() {
        dispatchProperty(KEY_DESTROY, EMPTY_PARAMETER);
    }

    // Loads Face AR effect
    fun loadEffect(effectName: String) {
        if (effectName == "") {
            unloadEffect()
        } else {
            dispatchProperty(KEY_LOAD_EFFECT, effectName);
        }
    }

    // Pauses applying Face AR effect
    fun pause() {
        dispatchProperty(KEY_PAUSE, EMPTY_PARAMETER);
    }

    // Resumes applying Face AR effect
    fun resume() {
        dispatchProperty(KEY_RESUME, EMPTY_PARAMETER);
    }

    private fun unloadEffect() {
        dispatchProperty(KEY_UNLOAD_EFFECT, EMPTY_PARAMETER);
    }

    private fun dispatchProperty(propertyKey: String, propertyValue: String) {
        callback.onExtensionPropertySet(propertyKey, propertyValue)
    }
}
