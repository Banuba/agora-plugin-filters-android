package com.banuba.sdk.agorapluginexample.widget.carousel

import android.view.LayoutInflater
import android.view.ViewGroup
import androidx.recyclerview.widget.DiffUtil
import androidx.recyclerview.widget.ListAdapter
import androidx.recyclerview.widget.RecyclerView
import com.banuba.sdk.agorapluginexample.R
import com.banuba.sdk.agorapluginexample.databinding.ItemCarouselEffectBinding
import com.banuba.agora.plugin.model.ArEffect

private val DIFF_CALLBACK = object : DiffUtil.ItemCallback<ArEffect>() {

    override fun areItemsTheSame(oldItem: ArEffect, newItem: ArEffect) = oldItem == newItem

    override fun areContentsTheSame(oldItem: ArEffect, newItem: ArEffect) = oldItem == newItem
}

class EffectsCarouselAdapter : ListAdapter<ArEffect, EffectsCarouselAdapter.EffectsViewHolder>(
    DIFF_CALLBACK
) {

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): EffectsViewHolder {
        val binding = ItemCarouselEffectBinding.inflate(
            LayoutInflater.from(parent.context),
            parent,
            false
        )
        return EffectsViewHolder(binding)
    }

    override fun getItemCount() = currentList.size

    override fun onBindViewHolder(holder: EffectsViewHolder, position: Int) {
        holder.onBind(getItem(position))
    }

    fun getItemAt(position: Int): ArEffect = getItem(position)

    class EffectsViewHolder(private val binding: ItemCarouselEffectBinding) :
        RecyclerView.ViewHolder(binding.root) {

        fun onBind(data: ArEffect) {
            if (data.preview == null) {
                binding.effectPreviewImage.setImageResource(R.drawable.ic_circle)
            } else {
                binding.effectPreviewImage.setImageBitmap(data.preview)
            }
        }
    }
}