package com.banuba.sdk.agorapluginexample

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_launcher.launchAgoraActivityButton

class LauncherActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_launcher)
        launchAgoraActivityButton.setOnClickListener {
            startActivity(Intent(this, MainActivity::class.java))
        }
    }
}
