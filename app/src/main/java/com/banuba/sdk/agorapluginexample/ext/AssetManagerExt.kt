package com.banuba.sdk.agorapluginexample.ext

import android.content.res.AssetManager
import java.io.File
import java.io.IOException

@Throws(IOException::class)
fun AssetManager.copyFile(sourcePath: String, dest: File) {
    open(sourcePath).use { assetsFile ->
        dest.outputStream().use { assetsFile.copyTo(it) }
    }
}