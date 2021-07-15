package com.banuba.sdk.agorapluginexample

import android.content.Context
import com.banuba.sdk.utils.ContextProvider
import com.banuba.utils.FileUtilsNN
import java.io.File
import java.util.zip.ZipFile

class BanubaResourceManager(private val context: Context) {

    companion object {
        private const val RESOURCES_ARCHIVE_NAME = "bnb-resources.zip"
        private const val RESOURCES_PATH = "bnb-resources"
        private const val RESOURCES_NN_PATH = "android_nn"
        private const val EFFECTS_PATH = "effects"

    }

    val resourcesPath: String
        get() = context.filesDir.absolutePath + "/$RESOURCES_PATH"

    fun prepare() {
        if (!File(resourcesPath).exists()) {
            unzipResources(resourcesPath)
            copyEffects(resourcesPath)
        }
        ContextProvider.setContext(context.applicationContext)
        FileUtilsNN.setContext(context.applicationContext)
        FileUtilsNN.setResourcesBasePath("$resourcesPath/$RESOURCES_NN_PATH")
    }

    private fun unzipResources(unzipPath: String) {
        val asset = context.assets.open(RESOURCES_ARCHIVE_NAME)
        val zipFile = File("$unzipPath.zip")
        val fout = zipFile.outputStream()
        asset.copyTo(fout)
        fout.close()
        asset.close()
        ZipFile(zipFile).use { zip ->
            zip.entries().asSequence().forEach { entry ->
                val file = File(unzipPath, entry.name)
                if (entry.isDirectory) {
                    file.mkdirs()
                } else {
                    zip.getInputStream(entry).use { input ->
                        file.outputStream().use { output ->
                            input.copyTo(output)
                        }
                    }
                }
            }
        }
    }

    private fun copyEffects(resPath: String) {
        val effectsPath = EFFECTS_PATH
        File(resPath, effectsPath).mkdirs()
        val effectsList = context.assets.list(effectsPath) ?: emptyArray()
        for (e in effectsList) {
            File(resPath, "$effectsPath/$e").mkdirs()
            val files = context.assets.list("$effectsPath/$e") ?: emptyArray()
            for (f in files) {
                val ofile = File(resPath, "$effectsPath/$e/$f")
                val ostream = ofile.outputStream()
                val istream = context.assets.open("$effectsPath/$e/$f")
                istream.copyTo(ostream)
                ostream.close()
                istream.close()
            }
        }
    }
}