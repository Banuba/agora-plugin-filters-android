package com.banuba.sdk.agorapluginexample

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import android.util.Log
import android.view.SurfaceView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import com.banuba.agora.plugin.BanubaEffectsLoader
import com.banuba.agora.plugin.BanubaExtensionManager
import com.banuba.agora.plugin.BanubaResourceManager
import com.banuba.agora.plugin.model.ArEffect
import com.banuba.sdk.agorapluginexample.widget.carousel.EffectsCarouselView
import io.agora.rtc2.Constants
import io.agora.rtc2.IMediaExtensionObserver
import io.agora.rtc2.IRtcEngineEventHandler
import io.agora.rtc2.RtcEngine
import io.agora.rtc2.RtcEngineConfig
import io.agora.rtc2.video.VideoCanvas
import io.agora.rtc2.video.VideoEncoderConfiguration
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    companion object {
        private const val REQUEST_CODE_PERMISSIONS = 1001

        private val REQUIRED_PERMISSIONS = arrayOf(
            Manifest.permission.CAMERA,
            Manifest.permission.RECORD_AUDIO
        )
    }

    private val banubaResourceManager by lazy(LazyThreadSafetyMode.NONE) {
        BanubaResourceManager(this)
    }

    private val videoEncoderConfiguration = VideoEncoderConfiguration(
        VideoEncoderConfiguration.VD_840x480,
        VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_30,
        VideoEncoderConfiguration.STANDARD_BITRATE,
        VideoEncoderConfiguration.ORIENTATION_MODE.ORIENTATION_MODE_FIXED_PORTRAIT
    )
    private val agoraRtc: RtcEngine by lazy(LazyThreadSafetyMode.NONE) {
        val config = RtcEngineConfig().apply {
            mContext = this@MainActivity
            mAppId = AGORA_APP_ID
            System.loadLibrary("banuba")
            addExtension(BanubaExtensionManager.EXTENSION_NAME)
            mEventHandler = agoraEventHandler
            mExtensionObserver = agoraExtensionObserver
        }
        RtcEngine.create(config)
    }
    private val agoraExtensionObserver = IMediaExtensionObserver { vendor, ext, key, value ->
        Log.d(vendor, "$key - $value")
    }
    private val agoraEventHandler = object : IRtcEngineEventHandler() {

        override fun onJoinChannelSuccess(channel: String, uid: Int, elapsed: Int) {
            runOnUiThread { agoraRtc.startPreview() }
        }

        override fun onFirstRemoteVideoDecoded(uid: Int, width: Int, height: Int, elapsed: Int) {
            runOnUiThread {
                val surfaceView = setupRemoteVideo(uid)
                remoteVideoContainer.removeAllViews()
                remoteVideoContainer.addView(surfaceView)
            }
        }

        override fun onUserOffline(uid: Int, reason: Int) {
            runOnUiThread { remoteVideoContainer.removeAllViews() }
        }
    }

    private val onEffectPrepared = object : BanubaResourceManager.EffectPreparedCallback {
        override fun onPrepared(effectName: String) {
            sendEffectToFilter(effectName)
        }
    }
    private val effectsCarouselCallback = object : EffectsCarouselView.ActionCallback {
        override fun onEffectsSelected(effect: ArEffect) {
            banubaResourceManager.prepareEffect(effect.name, onEffectPrepared)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        banubaResourceManager.prepare()
        effectsCarouselView.actionCallback = effectsCarouselCallback
        val effects = BanubaEffectsLoader(this).loadEffects()
        effectsCarouselView.setEffectsList(listOf(ArEffect.EMPTY) + effects, 0)
        if (checkAllPermissionsGranted()) {
            initAgoraEngine()
        } else {
            requestPermissions(REQUIRED_PERMISSIONS, REQUEST_CODE_PERMISSIONS)
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        agoraRtc.leaveChannel()
        RtcEngine.destroy()
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String>,
        results: IntArray
    ) {
        if (checkAllPermissionsGranted()) {
            initAgoraEngine()
        } else {
            Toast.makeText(applicationContext, "Please grant permission.", Toast.LENGTH_LONG).show()
            finish()
        }
        super.onRequestPermissionsResult(requestCode, permissions, results)
    }

    private fun initAgoraEngine() {
        agoraRtc.enableExtension(
            BanubaExtensionManager.VENDOR_NAME,
            BanubaExtensionManager.VIDEO_FILTER_NAME,
            true
        )
        val localSurfaceView = setupLocalVideo()
        localVideoContainer.addView(localSurfaceView)
        agoraRtc.setChannelProfile(Constants.CHANNEL_PROFILE_LIVE_BROADCASTING)
        agoraRtc.setClientRole(Constants.CLIENT_ROLE_BROADCASTER)
        agoraRtc.setVideoEncoderConfiguration(videoEncoderConfiguration)
        agoraRtc.enableVideo()
        agoraRtc.enableAudio()
        agoraRtc.joinChannel(AGORA_CLIENT_TOKEN, AGORA_CHANNEL_ID, null, 0)
        initBanubaPlugin()
    }

    private fun setupLocalVideo(): SurfaceView {
        val surfaceView = RtcEngine.CreateRendererView(this)
        surfaceView.setZOrderMediaOverlay(true)
        val videoCanvas = VideoCanvas(surfaceView, VideoCanvas.RENDER_MODE_HIDDEN, 0)
        agoraRtc.setupLocalVideo(videoCanvas)
        agoraRtc.setLocalRenderMode(
            Constants.RENDER_MODE_HIDDEN,
            Constants.VIDEO_MIRROR_MODE_DISABLED
        )
        return surfaceView
    }

    private fun setupRemoteVideo(uid: Int): SurfaceView {
        val surfaceView = RtcEngine.CreateRendererView(this)
        val videoCanvas = VideoCanvas(surfaceView, VideoCanvas.RENDER_MODE_HIDDEN, uid)
        agoraRtc.setupRemoteVideo(videoCanvas)
        return surfaceView
    }

    private fun initBanubaPlugin() {
        agoraRtc.setExtensionProperty(
            BanubaExtensionManager.VENDOR_NAME,
            BanubaExtensionManager.VIDEO_FILTER_NAME,
            BanubaExtensionManager.KEY_SET_RESOURCES_PATH,
            banubaResourceManager.resourcesPath
        )
        agoraRtc.setExtensionProperty(
            BanubaExtensionManager.VENDOR_NAME,
            BanubaExtensionManager.VIDEO_FILTER_NAME,
            BanubaExtensionManager.KEY_SET_EFFECTS_PATH,
            banubaResourceManager.effectsPath
        )
        agoraRtc.setExtensionProperty(
            BanubaExtensionManager.VENDOR_NAME,
            BanubaExtensionManager.VIDEO_FILTER_NAME,
            BanubaExtensionManager.KEY_SET_TOKEN,
            BANUBA_CLIENT_TOKEN
        )
    }

    private fun sendEffectToFilter(effect: String) {
        agoraRtc.setExtensionProperty(
            BanubaExtensionManager.VENDOR_NAME,
            BanubaExtensionManager.VIDEO_FILTER_NAME,
            BanubaExtensionManager.KEY_LOAD_EFFECT,
            effect
        )
    }

    private fun checkAllPermissionsGranted() = REQUIRED_PERMISSIONS.all {
        ContextCompat.checkSelfPermission(baseContext, it) == PackageManager.PERMISSION_GRANTED
    }
}
