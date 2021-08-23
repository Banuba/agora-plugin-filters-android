package com.banuba.sdk.agorapluginexample.ext

import android.view.View
import com.banuba.sdk.agorapluginexample.ext.ClickInterval
import com.banuba.sdk.agorapluginexample.ext.OneFirePerIntervalClickListener

fun View.setOneFirePerIntervalClickListener(
    interval: ClickInterval = ClickInterval.NORMAL,
    onClick: (View) -> Unit
) {
    val safeClickListener = OneFirePerIntervalClickListener(interval.milliseconds) {
        onClick(it)
    }
    setOnClickListener(safeClickListener)
}

fun View.gone() {
    visibility = View.GONE
}

fun View.visible() {
    visibility = View.VISIBLE
}