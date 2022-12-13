package com.banuba.sdk.agorapluginexample

import android.Manifest
import android.content.Context
import android.content.pm.PackageManager
import android.content.res.Configuration
import android.graphics.Matrix
import android.os.Bundle
import android.util.Log
import android.view.Surface
import android.view.SurfaceView
import android.view.View
import android.view.WindowManager
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import com.banuba.agora.plugin.AgoraExtension
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
        private const val KEY_SAVED_INSTANCE_EFFECT_INDEX = "lastLoadedEffectIndex"

        private val REQUIRED_PERMISSIONS = arrayOf(
            Manifest.permission.CAMERA,
            Manifest.permission.RECORD_AUDIO
        )
    }

    private val banubaResourceManager by lazy(LazyThreadSafetyMode.NONE) {
        BanubaResourceManager(this)
    }

    private val agoraRtc: RtcEngine by lazy(LazyThreadSafetyMode.NONE) {
        val config = RtcEngineConfig().apply {
            mContext = this@MainActivity
            mAppId = AGORA_APP_ID
            System.loadLibrary(banubaExtensionInterface.getLibraryName())
            addExtension(banubaExtensionInterface.getPluginName())
            ContextProvider.setContext(mContext)
            mEventHandler = agoraEventHandler
            mExtensionObserver = agoraExtensionObserver
        }
        Log.d("AgoraRtcEngine","Agora sdk version: ${RtcEngine.getSdkVersion()}")
        RtcEngine.create(config)
    }

    private val agoraExtensionObserver = object : IMediaExtensionObserver {
        override fun onEvent(provider: String?, extension: String?, key: String?, value: String?) {
            Log.d("ExtensionObserver","Extension event: $provider.$extension key:$key value:$value")
        }

        override fun onStarted(provider: String?, extension: String?) {
            Log.d("ExtensionObserver","Extension started: $provider.$extension")
        }

        override fun onStopped(provider: String?, extension: String?) {
            Log.d("ExtensionObserver","Extension stopped: $provider.$extension")
        }

        override fun onError(provider: String?, extension: String?, errCode: Int, errMsg: String?) {
            Log.d("ExtensionObserver","Extension error: $provider.$extension code:$errCode error:$errMsg")
        }
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

    private val banubaExtension = BanubaExtensionManager(object : AgoraInterface {
        override fun onSetExtensionProperty(provider: String, extension: String, propertyKey: String, propertyValue: String) {
            agoraRtc.setExtensionProperty(provider, extension, propertyKey, propertyValue)
        }
    })

    /* This interface separates banuba SDK and Agora rtc engine from each other. */
    private val banubaExtensionInterface: AgoraExtension = banubaExtension

    private val onEffectPrepared = object : BanubaResourceManager.EffectPreparedCallback {
        override fun onPrepared(effectName: String) {
            banubaExtension.loadEffect(effectName)
        }
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        initializeUI(savedInstanceState)
        requestPermissionsIfNecessaryAndStart()
    }

    override fun onDestroy() {
        super.onDestroy()
        shutdownBanubaExtension()
        shutdownAgora()
    }

    override fun onResume() {
        super.onResume()
        agoraRtc.enableVideo()
        agoraRtc.enableAudio()
        agoraRtc.startPreview()
        banubaExtension.resume()
    }

    override fun onPause() {
        super.onPause()
        agoraRtc.disableAudio()
        agoraRtc.disableVideo()
        agoraRtc.stopPreview()
        banubaExtension.pause()
    }

    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
        banubaExtension.setDeviceOrientation(getDeviceOrientationDegrees(this))
    }

    override fun onSaveInstanceState(savedInstanceState: Bundle) {
        super.onSaveInstanceState(savedInstanceState)
        savedInstanceState.putInt(KEY_SAVED_INSTANCE_EFFECT_INDEX, effectsCarouselView.selectedPosition);
    }

    private fun isPermissionsGranted() = REQUIRED_PERMISSIONS.all {
        ContextCompat.checkSelfPermission(baseContext, it) == PackageManager.PERMISSION_GRANTED
    }

    private fun requestPermissionsIfNecessaryAndStart() {
        checkPermissionsAndStart()
        if (!isPermissionsGranted()) {
            requestPermissions(REQUIRED_PERMISSIONS, REQUEST_CODE_PERMISSIONS)
            return
        }
    }

    private fun checkPermissionsAndStart() {
        if (isPermissionsGranted()) {
            initializeAgora()
            initializeBanubaExtension()
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, results: IntArray) {
        checkPermissionsAndStart()
        if (!isPermissionsGranted()) {
            Toast.makeText(applicationContext, "Please grant permission.", Toast.LENGTH_LONG).show()
            finish()
        }
        super.onRequestPermissionsResult(requestCode, permissions, results)
    }

    private fun initializeUI(savedInstanceState: Bundle?) {
        setContentView(R.layout.activity_main)
        banubaResourceManager.prepare()
        effectsCarouselView.actionCallback = object : EffectsCarouselView.ActionCallback {
            override fun onEffectsSelected(effect: ArEffect) {
                val name = effect.name
                banubaResourceManager.prepareEffect(name, onEffectPrepared)
            }
        }
        val effects = BanubaEffectsLoader(this).loadEffects()
        val position =
            if (savedInstanceState == null) 0
            else savedInstanceState.getInt(KEY_SAVED_INSTANCE_EFFECT_INDEX, 0)
        effectsCarouselView.setEffectsList(listOf(ArEffect.EMPTY) + effects, position)
    }

    private fun initializeAgora() {
        val localSurfaceView = setupLocalVideo()
        val videoEncoderConfiguration = VideoEncoderConfiguration(
            VideoEncoderConfiguration.VD_1280x720,
            VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_30,
            VideoEncoderConfiguration.STANDARD_BITRATE,
            VideoEncoderConfiguration.ORIENTATION_MODE.ORIENTATION_MODE_FIXED_PORTRAIT
        )
        if (isLandscape(this)) {
            localVideoContainerPortrait.visibility = View.GONE
            localVideoContainerLandscape.addView(localSurfaceView)
        } else {
            localVideoContainerPortrait.addView(localSurfaceView)
            localVideoContainerLandscape.visibility = View.GONE
        }
        agoraRtc.setChannelProfile(Constants.CHANNEL_PROFILE_LIVE_BROADCASTING)
        agoraRtc.setClientRole(Constants.CLIENT_ROLE_BROADCASTER)
        agoraRtc.setVideoEncoderConfiguration(videoEncoderConfiguration)
        agoraRtc.enableVideo()
        agoraRtc.enableAudio()
    }

    private fun initializeBanubaExtension() {
        agoraRtc.enableExtension(banubaExtensionInterface.getProviderName(), banubaExtensionInterface.getExtensionName(), true, Constants.MediaSourceType.PRIMARY_CAMERA_SOURCE)
        agoraRtc.joinChannel(AGORA_CLIENT_TOKEN, AGORA_CHANNEL_ID, null, 0)
        /* The initialize(...) method must be called once at application startup.
        * Only the very first call to this method is important.
        * All subsequent calls do nothing and do not affect anything. */
        banubaExtension.initialize(banubaResourceManager.resourcesPath, banubaResourceManager.effectsPath, BANUBA_CLIENT_TOKEN)
        banubaExtension.create()
        banubaExtension.setDeviceOrientation(getDeviceOrientationDegrees(this))
    }

    private fun setupLocalVideo(): SurfaceView {
        val surfaceView = SurfaceView(this)
        surfaceView.setZOrderMediaOverlay(true)
        val videoCanvas = VideoCanvas(surfaceView, VideoCanvas.RENDER_MODE_HIDDEN, 0)
        agoraRtc.setupLocalVideo(videoCanvas)
        agoraRtc.setLocalRenderMode(Constants.RENDER_MODE_HIDDEN, Constants.VIDEO_MIRROR_MODE_ENABLED)
        return surfaceView
    }

    private fun setupRemoteVideo(uid: Int): SurfaceView {
        val surfaceView = SurfaceView(this)
        val videoCanvas = VideoCanvas(surfaceView, VideoCanvas.RENDER_MODE_HIDDEN, uid)
        agoraRtc.setupRemoteVideo(videoCanvas)
        return surfaceView
    }

    private fun shutdownAgora() {
        agoraRtc.leaveChannel()
        RtcEngine.destroy()
    }

    private fun shutdownBanubaExtension() {
        banubaExtension.destroy()
        agoraRtc.enableExtension(banubaExtensionInterface.getProviderName(), banubaExtensionInterface.getExtensionName(), false)
    }

    /* Returns one of: 0 or 180 - portrait, 90 or 270 landscape left or right, */
    fun getDeviceOrientationDegrees(context: Context): Int {
        var degrees = 0
        val windowManager = context.getSystemService(WINDOW_SERVICE) as WindowManager
        val rotation = windowManager.defaultDisplay.rotation
        when (rotation) {
            Surface.ROTATION_0 -> degrees = 0
            Surface.ROTATION_90 -> degrees = 90
            Surface.ROTATION_180 -> degrees = 180
            Surface.ROTATION_270 -> degrees = 270
        }
        return degrees
    }

    fun isLandscape(context: Context): Boolean {
        val orientationDegrees = getDeviceOrientationDegrees(context)
        return orientationDegrees == 90 || orientationDegrees == 270
    }
}
