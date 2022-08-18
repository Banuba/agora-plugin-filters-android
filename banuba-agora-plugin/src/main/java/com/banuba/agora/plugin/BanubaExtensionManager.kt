package com.banuba.agora.plugin

class BanubaExtensionManager(agora: AgoraInterface) : AgoraExtension {
    private val agoraInterface: AgoraInterface = agora

    interface AgoraInterface {
        fun onSetExtensionProperty(provider: String, extension: String, propertyKey: String, propertyValue: String)
    }

    override fun getLibraryName(): String {
        return LIBRARY_NAME
    }

    override fun getPluginName(): String {
        return PLUGIN_NAME
    }

    override fun getProviderName(): String {
        return PROVIDER_NAME
    }

    override fun getExtensionName(): String {
        return EXTENSION_NAME
    }

    fun create(resourcesPath: String, effectsPath: String, clientToken: String) {
        sendProperty(CALL_SET_RESOURCES_PATH, resourcesPath);
        sendProperty(CALL_SET_EFFECTS_PATH, effectsPath);
        sendProperty(CALL_SET_CLIENT_TOKEN, clientToken);
        sendProperty(CALL_INITIALIZE, EMPTY_PARAMETER)
        sendProperty(CALL_CREATE, EMPTY_PARAMETER);
    }

    fun destroy() {
        sendProperty(CALL_DESTROY, EMPTY_PARAMETER);
    }

    fun loadEffect(effectName: String) {
        if (effectName == "") {
            unloadEffect()
        } else {
            sendProperty(CALL_LOAD_EFFECT, effectName);
        }
    }

    fun unloadEffect() {
        sendProperty(CALL_UNLOAD_EFFECT, EMPTY_PARAMETER);
    }

    fun pause() {
        sendProperty(CALL_PAUSE, EMPTY_PARAMETER);
    }

    fun resume() {
        sendProperty(CALL_RESUME, EMPTY_PARAMETER);
    }

    fun setDeviceOrientation(angle: Int) {
        sendProperty(CALL_SET_DEVICE_ORIENTATION, angle.toString())
    }

    private fun sendProperty(propertyKey: String, propertyValue: String) {
        agoraInterface.onSetExtensionProperty(PROVIDER_NAME, EXTENSION_NAME, propertyKey, propertyValue)
    }

    private companion object {
        const val LIBRARY_NAME = "banuba"
        const val PLUGIN_NAME = "banuba-plugin" // CMake target
        const val PROVIDER_NAME = "Banuba"
        const val EXTENSION_NAME = "BanubaFilter"

        const val CALL_INITIALIZE = "initialize"
        const val CALL_CREATE = "create"
        const val CALL_DESTROY = "destroy"
        const val CALL_LOAD_EFFECT = "load_effect"
        const val CALL_UNLOAD_EFFECT = "unload_effect"
        const val CALL_PAUSE = "pause"
        const val CALL_RESUME = "resume"
        const val CALL_SET_DEVICE_ORIENTATION = "set_device_orientation"
        const val CALL_SET_RESOURCES_PATH = "set_resources_path"
        const val CALL_SET_EFFECTS_PATH = "set_effects_path"
        const val CALL_SET_CLIENT_TOKEN = "set_client_token"
        const val EMPTY_PARAMETER = "1" // an empty parameter must contain a value, otherwise the agora won't process it
    }
}
