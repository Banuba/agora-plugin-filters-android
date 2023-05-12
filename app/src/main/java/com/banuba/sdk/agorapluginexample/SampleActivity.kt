package com.banuba.sdk.agorapluginexample

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_sample.*


class SampleActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_sample)
        startSampleButton.setOnClickListener {
            startActivity(Intent(this, MainActivity::class.java))
        }
    }
}
