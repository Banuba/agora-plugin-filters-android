package com.banuba.sdk.agorapluginexample.widget.carousel

import android.graphics.Rect
import android.view.View
import androidx.annotation.Px
import androidx.recyclerview.widget.RecyclerView

class CenterOutItemsDecoration(
    @Px itemWidth: Int,
    @Px itemMargin: Int,
    @Px parentWidth: Int
) : RecyclerView.ItemDecoration() {
    private val padding = parentWidth / 2 - itemWidth / 2 - itemMargin

    override fun getItemOffsets(
        outRect: Rect,
        view: View,
        parent: RecyclerView,
        state: RecyclerView.State
    ) {
        val position = parent.getChildViewHolder(view).adapterPosition
        if (position == 0) {
            outRect.left = padding
        }
        if (position == state.itemCount - 1) {
            outRect.right = padding
        }
    }
}