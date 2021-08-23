package com.banuba.sdk.agorapluginexample.widget

import android.content.Context
import android.graphics.Canvas
import android.graphics.Path
import android.graphics.RectF
import android.util.AttributeSet
import androidx.appcompat.widget.AppCompatImageView
import com.banuba.sdk.agorapluginexample.R

class RoundedImageView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : AppCompatImageView(context, attrs, defStyleAttr) {

    companion object {
        private const val RADIUS_DEFAULT_VALUE = -1F
    }

    private var radius = RADIUS_DEFAULT_VALUE
        set(value) {
            field = value
            isPathInitialized = false
            invalidate()
        }

    private var clipPath: Path = Path()
    private var isPathInitialized = false

    init {
        val attributes = context.obtainStyledAttributes(attrs, R.styleable.RoundedImageView)
        radius = attributes.getDimension(R.styleable.RoundedImageView_radius, RADIUS_DEFAULT_VALUE)
        attributes.recycle()
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        isPathInitialized = false
    }

    override fun onDraw(canvas: Canvas) {
        if (!isPathInitialized) {
            initPath()
            isPathInitialized = true
        }
        canvas.clipPath(clipPath)
        super.onDraw(canvas)
    }

    private fun initPath() {
        clipPath.apply {
            reset()
            val rect = RectF(
                paddingStart.toFloat(),
                paddingTop.toFloat(),
                (width - paddingEnd).toFloat(),
                (height - paddingBottom).toFloat()
            )
            val resultRadius = if (radius == RADIUS_DEFAULT_VALUE) width.toFloat() / 2 else radius
            addRoundRect(rect, resultRadius, resultRadius, Path.Direction.CW)
        }
    }
}