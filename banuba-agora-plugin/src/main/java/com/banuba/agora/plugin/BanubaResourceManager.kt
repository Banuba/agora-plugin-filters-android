package com.banuba.agora.plugin

import android.content.Context
import android.util.Log
import com.banuba.agora.plugin.ext.copyFile
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.io.File
import java.io.IOException
import java.util.zip.ZipFile

class BanubaResourceManager(private val context: Context) {

    interface EffectPreparedCallback {
        fun onPrepared(effectName: String)
    }

    companion object {
        private const val TAG = "BanubaResourceManager"

        private const val RESOURCES_ARCHIVE_NAME = "bnb-resources.zip"
        private const val RESOURCES_CHECKSUM_FILE = "zip_checksum"

        private const val RESOURCES_PATH = "bnb-resources"
        private const val RESOURCES_NN_PATH = "android_nn"
        const val EFFECTS_PATH = "effects"
    }

    val resourcesPath: String
        get() = "${context.filesDir.absolutePath}/${RESOURCES_PATH}"
    val effectsPath: String
        get() = "$resourcesPath/${EFFECTS_PATH}"

    private val assetManager = context.assets

    private val scope = CoroutineScope(Dispatchers.IO)

    fun prepare() {
        prepareResources(File(resourcesPath))
        File(effectsPath).run {
            deleteRecursively()
            mkdirs()
        }
    }

    fun prepareEffect(name: String, callback: EffectPreparedCallback) {
        val deviceDirPath = "$effectsPath/$name"
        val assetsDirPath = "${EFFECTS_PATH}/$name"
        scope.launch {
            copyAssets(assetsDirPath, deviceDirPath)
            callback.onPrepared(name)
        }
    }

    private fun prepareResources(destDir: File) {
        if (!shouldCopyResources(destDir)) return
        destDir.deleteRecursively()
        destDir.mkdir()
        listOf(
            RESOURCES_CHECKSUM_FILE,
            RESOURCES_ARCHIVE_NAME
        ).forEach {
            copyFile(it, File(destDir, it))
        }
        val zipFile = File(destDir, RESOURCES_ARCHIVE_NAME)
        unzip(destDir, zipFile)
        zipFile.delete()
    }

    private fun shouldCopyResources(destDir: File): Boolean = try {
        val checksumCurrent = File(destDir, RESOURCES_CHECKSUM_FILE).readBytes()
        val referenceChecksum = assetManager.open(RESOURCES_CHECKSUM_FILE).readBytes()
        !checksumCurrent.contentEquals(referenceChecksum)
    } catch (e: IOException) {
        Log.w(TAG, "shouldCopyResources", e)
        true
    }

    private fun unzip(destDir: File, zipFile: File) = try {
        ZipFile(zipFile).use { zip ->
            zip.entries().asSequence().forEach { entry ->
                val targetFile = File(destDir, entry.name)
                if (entry.isDirectory) {
                    targetFile.mkdirs()
                } else {
                    zip.getInputStream(entry).use { input ->
                        targetFile.outputStream().use { output ->
                            input.copyTo(output)
                        }
                    }
                }
            }
        }
    } catch (e: IOException) {
        Log.w(TAG, "unzip error", e)
    }

    private fun copyAssets(from: String, to: String) {
        val files = context.assets.list(from) ?: emptyArray()
        File(to).mkdirs()
        files.forEach {
            val absoluteAssetsPath = "$from/$it"
            if (context.assets.list(absoluteAssetsPath)?.size != 0) {
                copyAssets(
                    absoluteAssetsPath,
                    "$to/$it"
                )
            } else {
                copyFile(absoluteAssetsPath, File("$to/$it"))
            }
        }
    }

    private fun copyFile(sourcePath: String, destFile: File) = try {
        assetManager.copyFile(sourcePath = sourcePath, dest = destFile)
        true
    } catch (e: IOException) {
        Log.w(TAG, "Could not copy file $sourcePath", e)
        false
    }
}
