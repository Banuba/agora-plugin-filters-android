package com.banuba.sdk.agorapluginexample.widget.carousel

import android.view.View
import androidx.recyclerview.widget.LinearSnapHelper
import androidx.recyclerview.widget.OrientationHelper
import androidx.recyclerview.widget.RecyclerView
import kotlin.math.abs

class HorizontalCenterSnapHelper : LinearSnapHelper() {

    private var horizontalHelper: OrientationHelper? = null

    private val scrollListener = object : RecyclerView.OnScrollListener() {

        private var isScrollInProgress = false

        override fun onScrollStateChanged(recyclerView: RecyclerView, newState: Int) {
            isScrollInProgress =
                if (newState == RecyclerView.SCROLL_STATE_IDLE && !isScrollInProgress) {
                    val layoutManager = recyclerView.layoutManager ?: return
                    val view = findSnapView(layoutManager)
                    if (view != null) {
                        val out = calculateDistanceToFinalSnap(layoutManager, view)
                        recyclerView.smoothScrollBy(out[0], out[1])
                    }
                    true
                } else {
                    false
                }
        }
    }

    override fun attachToRecyclerView(recyclerView: RecyclerView?) {
        super.attachToRecyclerView(recyclerView)
        recyclerView?.addOnScrollListener(scrollListener)
    }

    override fun findSnapView(layoutManager: RecyclerView.LayoutManager) =
        findCenterView(layoutManager)

    override fun calculateDistanceToFinalSnap(
        layoutManager: RecyclerView.LayoutManager,
        targetView: View
    ) = IntArray(2).apply {
        this[0] = calculateDistanceToCenter(targetView, layoutManager)
    }

    private fun findCenterView(
        layoutManager: RecyclerView.LayoutManager
    ): View? {
        val childCount = layoutManager.childCount
        if (childCount == 0) return null

        val centerContainer: Int = getCenterOfContainer(layoutManager)

        var closestChild: View? = null
        var absClosest = Float.MAX_VALUE
        for (i in 0 until childCount) {
            val child = layoutManager.getChildAt(i) ?: continue
            val childCenter = child.x + child.width / 2
            val absDistance = abs(childCenter - centerContainer)
            if (absDistance < absClosest) {
                absClosest = absDistance
                closestChild = child
            }
        }

        return closestChild
    }

    private fun calculateDistanceToCenter(
        targetView: View,
        layoutManager: RecyclerView.LayoutManager
    ): Int {
        val childCenter = (targetView.x + targetView.width / 2).toInt()
        val containerCenter = getCenterOfContainer(layoutManager)
        return childCenter - containerCenter
    }

    private fun getCenterOfContainer(layoutManager: RecyclerView.LayoutManager): Int {
        val helper = getHorizontalHelper(layoutManager)
        return if (layoutManager.clipToPadding) {
            helper.startAfterPadding + helper.totalSpace / 2
        } else {
            helper.end / 2
        }
    }

    private fun getHorizontalHelper(
        layoutManager: RecyclerView.LayoutManager
    ): OrientationHelper {
        if (horizontalHelper == null || horizontalHelper!!.layoutManager !== layoutManager) {
            horizontalHelper = OrientationHelper.createHorizontalHelper(layoutManager)
        }
        return horizontalHelper!!
    }
}