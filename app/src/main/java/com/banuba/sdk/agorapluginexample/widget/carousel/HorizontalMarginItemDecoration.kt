package com.banuba.sdk.agorapluginexample.widget.carousel

import android.graphics.Rect
import android.view.View
import androidx.annotation.Px
import androidx.recyclerview.widget.RecyclerView

class HorizontalMarginItemDecoration(
    @Px betweenSpace: Int,
    @Px private val topMargin: Int,
    @Px private val bottomMargin: Int
) : RecyclerView.ItemDecoration() {

    private val halfBetweenSpace = betweenSpace / 2

    override fun getItemOffsets(
        outRect: Rect,
        view: View,
        parent: RecyclerView,
        state: RecyclerView.State
    ) {
        with(outRect) {
            top = topMargin
            bottom = bottomMargin
            left = halfBetweenSpace
            right = halfBetweenSpace
        }
    }
}