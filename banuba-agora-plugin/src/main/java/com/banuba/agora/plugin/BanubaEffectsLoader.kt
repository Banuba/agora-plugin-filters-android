package com.banuba.agora.plugin

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import com.banuba.agora.plugin.model.ArEffect

class BanubaEffectsLoader(private val context: Context) {

    fun loadEffects(): List<ArEffect> {
        val effectsAssetsPath = BanubaResourceManager.EFFECTS_PATH
        val effectsAssetsDir = context.assets.list(effectsAssetsPath)?.toList() ?: emptyList()
        return effectsAssetsDir.map { effectName ->
            val previewPath = "$effectsAssetsPath/$effectName/preview.png"
            ArEffect(
                name = effectName,
                preview = createBitmapFromAssetsPath(previewPath)
            )
        }
    }

    private fun createBitmapFromAssetsPath(path: String): Bitmap? =
        try {
            context.assets.open(path).use { stream ->
                BitmapFactory.decodeStream(stream)
            }
        } catch (e: Throwable) {
            null
        }
}
