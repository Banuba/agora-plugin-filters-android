package com.banuba.sdk.agorapluginexample

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.banuba.sdk.agorapluginexample.databinding.ActivitySampleBinding

class SampleActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        var binding = ActivitySampleBinding.inflate(layoutInflater)
        val view = binding.root
        setContentView(view)
        binding.startSampleButton.setOnClickListener {
            startActivity(Intent(this, MainActivity::class.java))
        }
    }
}
