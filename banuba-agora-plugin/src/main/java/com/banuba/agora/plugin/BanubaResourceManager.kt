package com.banuba.agora.plugin

import android.content.Context
import android.content.res.AssetManager
import android.util.Log
import java.io.File
import java.io.IOException
import java.util.zip.ZipFile

/**
 * Class is responsible for preparing Face AR effects located in Android assets folder.
 * It is strongly recommended storing effects in assets/effects folder.
 */
class BanubaResourceManager(context: Context) {

    companion object {
        private const val TAG = "BanubaResourceManager"

        private const val RESOURCES_ARCHIVE_NAME = "bnb-resources.zip"
        private const val RESOURCES_CHECKSUM_FILE = "zip_checksum"
        private const val RESOURCES_PATH = "bnb-resources" //USED?
        private const val EFFECTS_PATH = "effects"
    }

    val resourcesPath: String = "${context.filesDir.absolutePath}/${RESOURCES_PATH}"
    val effectsPath: String = "$resourcesPath/${EFFECTS_PATH}"
    var onEffectReadyCallback: (String) -> Unit = {}

    private val assetManager = context.assets

    /**
     * Prepares all resources required for Banuba Face AR and extension.
     * NOTE:
     * Consider running in background thread
     */
    fun init() {
        prepareResources(File(resourcesPath))
        File(effectsPath).run {
            deleteRecursively()
            mkdirs()
        }
    }

    /**
     * Prepares passed effect for usage.
     * NOTE:
     * Consider running in background thread
     */
    fun prepareEffect(name: String) {
        val deviceDirPath = "$effectsPath/$name"
        val assetsDirPath = "${EFFECTS_PATH}/$name"
        copyAssets(assetsDirPath, deviceDirPath)
        onEffectReadyCallback(name)
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
        Log.w(TAG, "Error: should copy resources", e)
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
        Log.w(TAG, "Could not unzip file", e)
    }

    private fun copyAssets(
        from: String,
        to: String
    ) {
        val files = assetManager.list(from) ?: emptyArray()
        File(to).mkdirs()
        files.forEach {
            val absoluteAssetsPath = "$from/$it"
            if (assetManager.list(absoluteAssetsPath)?.size != 0) {
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
        Log.w(TAG, "Could not copy file = $sourcePath", e)
        false
    }

    @Throws(IOException::class)
    private fun AssetManager.copyFile(sourcePath: String, dest: File) {
        open(sourcePath).use { assetsFile ->
            dest.outputStream().use { assetsFile.copyTo(it) }
        }
    }
}
