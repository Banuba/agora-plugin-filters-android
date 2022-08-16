package com.banuba.sdk.agorapluginexample

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.content.res.Configuration
import android.os.Bundle
import android.util.Log
import android.view.Surface
import android.view.SurfaceView
import android.view.WindowManager
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import com.banuba.agora.plugin.BanubaEffectsLoader
import com.banuba.agora.plugin.BanubaExtensionManager
import com.banuba.agora.plugin.BanubaExtensionManager.AgoraInterface
import com.banuba.agora.plugin.BanubaResourceManager
import com.banuba.agora.plugin.model.ArEffect
import com.banuba.sdk.agorapluginexample.widget.carousel.EffectsCarouselView
import com.banuba.sdk.utils.ContextProvider
import io.agora.rtc2.*
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
            addExtension("banuba-plugin")
            ContextProvider.setContext(mContext)
            mEventHandler = agoraEventHandler
            mExtensionObserver = agoraExtensionObserver
        }
        Log.d("AgoraRtcEngine","Agora sdk version: ${RtcEngine.getSdkVersion()}")
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

    private val banubaExtension = BanubaExtensionManager(object : BanubaExtensionManager.AgoraInterface {
        override fun onEnableExtension(provider: String, extension: String, enable: Boolean) {
            agoraRtc.enableExtension(provider, extension, enable)
            if (enable) {
                val localSurfaceView = setupLocalVideo()
                localVideoContainer.addView(localSurfaceView)
                agoraRtc.setChannelProfile(Constants.CHANNEL_PROFILE_LIVE_BROADCASTING)
                agoraRtc.setClientRole(Constants.CLIENT_ROLE_BROADCASTER)
                agoraRtc.setVideoEncoderConfiguration(videoEncoderConfiguration)
                agoraRtc.enableVideo()
                agoraRtc.enableAudio()
                agoraRtc.joinChannel(AGORA_CLIENT_TOKEN, AGORA_CHANNEL_ID, null, 0)
            }
        }

        override fun onSetExtensionProperty(provider: String, extension: String, propertyKey: String, propertyValue: String) {
            agoraRtc.setExtensionProperty(provider, extension, propertyKey, propertyValue)
        }
    })

    private val onEffectPrepared = object : BanubaResourceManager.EffectPreparedCallback {
        override fun onPrepared(effectName: String) {
            banubaExtension.loadEffect(effectName)
        }
    }

    private val effectsCarouselCallback = object : EffectsCarouselView.ActionCallback {
        override fun onEffectsSelected(effect: ArEffect) {
            banubaResourceManager.prepareEffect(effect.name, onEffectPrepared);
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

    override fun onPause() {
        super.onPause()
        banubaExtension.pause();
    }

    override fun onResume() {
        super.onResume()
        banubaExtension.resume();
    }

    override fun onDestroy() {
        super.onDestroy()
        banubaExtension.destroy()
        agoraRtc.leaveChannel()
        RtcEngine.destroy()
    }

    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
        banubaExtension.setDeviceOrientation(getDeviceOrientationDegrees())
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
        banubaExtension.create(banubaResourceManager.resourcesPath, banubaResourceManager.effectsPath, BANUBA_CLIENT_TOKEN);
        banubaExtension.setDeviceOrientation(getDeviceOrientationDegrees())
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


    private fun checkAllPermissionsGranted() = REQUIRED_PERMISSIONS.all {
        ContextCompat.checkSelfPermission(baseContext, it) == PackageManager.PERMISSION_GRANTED
    }

    private fun getDeviceOrientationDegrees(): Int {
        when (getDeviceOrientationConst()) {
            Surface.ROTATION_90 -> return 90
            Surface.ROTATION_180 -> return 180
            Surface.ROTATION_270 -> return 270
            else -> return 0; // Surface.ROTATION_0
        }
    }

    /* Returns one of: Surface.ROTATION_0; Surface.ROTATION_90; Surface.ROTATION_180; Surface.ROTATION_270 */
    private fun getDeviceOrientationConst(): Int {
        return (this.getSystemService(Context.WINDOW_SERVICE) as WindowManager).defaultDisplay.rotation
    }
}
