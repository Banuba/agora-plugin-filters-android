package com.banuba.agora.plugin.model

import android.graphics.Bitmap

data class ArEffect(
    val name: String,
    val preview: Bitmap?
) {
    companion object {
        val EMPTY = ArEffect("", null)
    }
}
