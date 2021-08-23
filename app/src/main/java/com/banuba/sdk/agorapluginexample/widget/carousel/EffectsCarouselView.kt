package com.banuba.sdk.agorapluginexample.widget.carousel

import android.content.Context
import android.util.AttributeSet
import android.view.LayoutInflater
import androidx.constraintlayout.widget.ConstraintLayout
import androidx.core.view.doOnLayout
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.banuba.sdk.agorapluginexample.R
import com.banuba.sdk.agorapluginexample.databinding.ViewEffectsCarouselBinding
import com.banuba.sdk.agorapluginexample.ext.dimenPx
import com.banuba.sdk.agorapluginexample.ext.setOneFirePerIntervalClickListener
import com.banuba.sdk.agorapluginexample.model.ArEffect
import kotlin.math.abs

class EffectsCarouselView @JvmOverloads constructor(
    context: Context, attrs: AttributeSet? = null, defStyleAttr: Int = 0
) : ConstraintLayout(context, attrs, defStyleAttr) {

    interface ActionCallback {

        fun onEffectsSelected(effect: ArEffect)

        fun onClose() {}
    }

    companion object {
        private const val SELECTED_DISTANCE_ACCURACY = 0.1
    }

    var actionCallback: ActionCallback? = null

    private val effectsAdapter = EffectsCarouselAdapter()
    private val snapHelper = HorizontalCenterSnapHelper()


    private val recyclerViewScrollListener = object : RecyclerView.OnScrollListener() {

        private val itemMargin = context.dimenPx(R.dimen.effects_carousel_item_margin)
        private val itemWidth = context.dimenPx(R.dimen.effect_preview_size)
        private val selectedItemWidth = context.dimenPx(R.dimen.effects_preview_selected_size)

        private val maxDistance = itemMargin / 2 + itemWidth / 2
        private val selectedItemScale = selectedItemWidth.toFloat() / itemWidth

        private var selectedPosition = -1

        override fun onScrolled(recyclerView: RecyclerView, dx: Int, dy: Int) {
            val layoutManager = recyclerView.layoutManager ?: return
            val targetView = snapHelper.findSnapView(layoutManager) ?: return
            val remainingDistance =
                snapHelper.calculateDistanceToFinalSnap(layoutManager, targetView)[0]

            val scaleFactor = (maxDistance.toFloat() - abs(remainingDistance)) / maxDistance
            val currentScale = 1F + (selectedItemScale - 1F) * scaleFactor
            targetView.scaleX = currentScale
            targetView.scaleY = currentScale

            val newPosition = layoutManager.getPosition(targetView)
            if (abs(remainingDistance) > SELECTED_DISTANCE_ACCURACY * SELECTED_DISTANCE_ACCURACY || newPosition == selectedPosition) return
            selectedPosition = newPosition
            onEffectSelected(selectedPosition)
        }
    }

    private val binding =
        ViewEffectsCarouselBinding.inflate(LayoutInflater.from(context), this)

    init {
        clipChildren = false
        setupRecyclerView()
        binding.effectsCarouselExitButton.setOneFirePerIntervalClickListener {
            actionCallback?.onClose()
        }
    }

    fun setEffectsList(
        ars: List<ArEffect>, checkedPosition: Int = if (ars.isNotEmpty()) 0 else -1
    ) {
        effectsAdapter.submitList(ars) {
            if (checkedPosition != -1) {
                binding.effectsCarouselRecyclerView.scrollToPosition(checkedPosition)
            }
        }
    }

    private fun onEffectSelected(position: Int) {
        val selectedEffect = effectsAdapter.getItemAt(position)
        actionCallback?.onEffectsSelected(selectedEffect)
    }

    private fun setupRecyclerView() {
        doOnLayout {
            addRecyclerItemDecorations()
            binding.effectsCarouselRecyclerView.invalidate()
        }
        val layoutManager = LinearLayoutManager(context, RecyclerView.HORIZONTAL, false)
        binding.effectsCarouselRecyclerView.layoutManager = layoutManager
        snapHelper.attachToRecyclerView(binding.effectsCarouselRecyclerView)
        binding.effectsCarouselRecyclerView.addOnScrollListener(recyclerViewScrollListener)
        binding.effectsCarouselRecyclerView.adapter = effectsAdapter
        binding.effectsCarouselRecyclerView.itemAnimator = null
    }

    private fun addRecyclerItemDecorations() {
        val itemMargin = context.dimenPx(R.dimen.effects_carousel_item_margin)
        val itemWidth = context.dimenPx(R.dimen.effect_preview_size)
        val centerOutItemsDecoration = CenterOutItemsDecoration(
            itemWidth = itemWidth,
            itemMargin = itemMargin / 2,
            parentWidth = width
        )
        val marginDecoration = HorizontalMarginItemDecoration(
            betweenSpace = itemMargin, topMargin = 0, bottomMargin = 0
        )
        binding.effectsCarouselRecyclerView.addItemDecoration(centerOutItemsDecoration)
        binding.effectsCarouselRecyclerView.addItemDecoration(marginDecoration)
    }
}