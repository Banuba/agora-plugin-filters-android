package com.banuba.agora.plugin.ext

import android.content.res.AssetManager
import java.io.File
import java.io.IOException

@Throws(IOException::class)
internal fun AssetManager.copyFile(sourcePath: String, dest: File) {
    open(sourcePath).use { assetsFile ->
        dest.outputStream().use { assetsFile.copyTo(it) }
    }
}
