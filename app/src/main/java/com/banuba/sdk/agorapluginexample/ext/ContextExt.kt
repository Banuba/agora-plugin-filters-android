package com.banuba.sdk.agorapluginexample.ext

import android.content.Context
import androidx.annotation.DimenRes

fun Context.dimenPx(@DimenRes dimen: Int) =
    resources.getDimensionPixelSize(dimen)
