package com.banuba.agora.plugin

interface AgoraExtension {
    fun getLibraryName(): String
    fun getPluginName(): String
    fun getProviderName(): String
    fun getExtensionName(): String
}
