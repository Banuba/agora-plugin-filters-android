package com.banuba.sdk.agorapluginexample

import android.Manifest
import android.annotation.SuppressLint
import android.content.pm.PackageManager
import android.os.Bundle
import android.util.Log
import android.view.SurfaceView
import android.view.View
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.content.ContextCompat
import com.banuba.android.sdk.ext.agora.BanubaExtensionManager
import com.banuba.android.sdk.ext.agora.BanubaExtensionManager.BANUBA_EXTENSION_NAME
import com.banuba.android.sdk.ext.agora.BanubaExtensionManager.BANUBA_PLUGIN_NAME
import com.banuba.android.sdk.ext.agora.BanubaExtensionManager.BANUBA_PROVIDER_NAME
import com.banuba.sdk.agorapluginexample.databinding.ActivityMainBinding
import io.agora.rtc2.*
import io.agora.rtc2.video.VideoCanvas
import io.agora.rtc2.video.VideoEncoderConfiguration

class MainActivity : AppCompatActivity() {
    companion object {
        private const val TAG = "BanubaExtension"

        private const val REQUEST_CODE_PERMISSIONS = 1001

        private const val UID_LOCAL = 0

        private val REQUIRED_PERMISSIONS = arrayOf(
            Manifest.permission.CAMERA,
            Manifest.permission.RECORD_AUDIO
        )
    }

    private lateinit var binding: ActivityMainBinding

    // Use list of Face AR effects available in assets/effects folder
    private val availableEffects = listOf(
        "360_CubemapEverest_noSound",
        "CartoonOctopus",
        "clubs",
        "frame1",
        "Graduate",
        "prequel_VE",
        "RainbowBeauty",
        "Regular_blur",
        "Regular_Dawn_of_nature",
        "relook",
        "Sunset",
        "TrollGrandma",
        "video_BG_Metro_sfx",
        "video_BG_RainyCafe",
        "WhooshBeautyFemale"
    )

    private var effectIndex = -1

    private var isJoinedToChannel = false

    private val agoraVideoConfiguration = VideoEncoderConfiguration(
        VideoEncoderConfiguration.VD_1280x720,
        VideoEncoderConfiguration.FRAME_RATE.FRAME_RATE_FPS_30,
        VideoEncoderConfiguration.STANDARD_BITRATE,
        VideoEncoderConfiguration.ORIENTATION_MODE.ORIENTATION_MODE_FIXED_PORTRAIT
    )

    private val agoraConfig: RtcEngineConfig by lazy(LazyThreadSafetyMode.NONE) {
        RtcEngineConfig().apply {
            mContext = applicationContext
            mAppId = AGORA_APP_ID
            // Add Banuba Extension
            addExtension(BANUBA_PLUGIN_NAME)
            mEventHandler = agoraEventHandler
            mExtensionObserver = agoraExtensionObserver

            Log.d(TAG, "Agora sdk version: ${RtcEngine.getSdkVersion()}")
        }
    }

    private val agoraRtc: RtcEngine by lazy(LazyThreadSafetyMode.NONE) {
        RtcEngine.create(agoraConfig).apply {
            setChannelProfile(Constants.CHANNEL_PROFILE_LIVE_BROADCASTING)
            setClientRole(Constants.CLIENT_ROLE_BROADCASTER)
            setVideoEncoderConfiguration(agoraVideoConfiguration)
            enableVideo()
            enableAudio()
        }
    }

    private val agoraExtensionObserver = object : IMediaExtensionObserver {
        override fun onEvent(
            provider: String?,
            extension: String?,
            key: String?,
            value: String?
        ) {
            Log.d(TAG, "Extension event: $provider.$extension key:$key value:$value")
        }

        override fun onStarted(
            provider: String?,
            extension: String?
        ) {
            Log.d(TAG, "Extension started: $provider.$extension")
            if (BANUBA_PROVIDER_NAME == provider && BANUBA_EXTENSION_NAME == extension) {
                runOnUiThread { showToast("Banuba extension started!") }
            }
        }

        override fun onStopped(
            provider: String?,
            extension: String?
        ) {
            Log.d(TAG, "Extension stopped: $provider.$extension")
            if (BANUBA_PROVIDER_NAME == provider && BANUBA_EXTENSION_NAME == extension) {
                runOnUiThread { showToast("Banuba extension stopped!") }
            }
        }

        override fun onError(
            provider: String?,
            extension: String?,
            errCode: Int,
            errMsg: String?
        ) {
            Log.e(TAG, "Extension error: $provider.$extension code:$errCode error:$errMsg")
        }
    }

    private val agoraEventHandler = object : IRtcEngineEventHandler() {
        override fun onJoinChannelSuccess(channel: String, uid: Int, elapsed: Int) {
            Log.d(TAG, "User joiner to channel = $channel")
        }

        override fun onFirstRemoteVideoDecoded(uid: Int, width: Int, height: Int, elapsed: Int) {
            Log.d(TAG, "User received remote video from user = $uid")
            runOnUiThread { addRemoteVideo(uid) }
        }

        override fun onUserOffline(uid: Int, reason: Int) {
            Log.d(TAG, "User = $uid is offline")
            runOnUiThread { removeRemoteVideo() }
        }
    }

    private var isFrontCamera = true

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        val view = binding.root
        setContentView(view)

        invalidateUiState()

        binding.joinButton.setOnClickListener {
            joinChannel(AGORA_CHANNEL)
        }

        binding.switchCameraButton.setOnClickListener {
            agoraRtc.switchCamera()
            isFrontCamera = !isFrontCamera
            BanubaExtensionManager.enableMirroring(isFrontCamera)
        }
        binding.leaveButton.setOnClickListener { leaveChannel() }
        binding.applyEffectButton.setOnClickListener {
            toggleEffect()

            getCurrentEffect().let { effectName ->
                // effectName is empty when effect is canceled
                Log.d(TAG, "Prepare effect = $effectName")
                // Consider executing method in background thread
                BanubaExtensionManager.loadEffect(effectName)
            }
        }

        // Enable BanubaFilters extension
        enableBanubaExtension(true)
    }

    @SuppressLint("NewApi")
    override fun onStart() {
        super.onStart()
        Log.d(TAG, "onStart")
        startRtcAndExtensionOr {
            Log.d(TAG, "Request permissions")
            requestPermissions(REQUIRED_PERMISSIONS, REQUEST_CODE_PERMISSIONS)
        }
    }

    override fun onStop() {
        super.onStop()
        Log.d(TAG, "onStop")
        agoraRtc.stopPreview()
        leaveChannel()
    }

    override fun onResume() {
        super.onResume()
        // Required to resume current Banuba Face AR effect
        BanubaExtensionManager.resume()
    }

    override fun onPause() {
        super.onPause()

        // Required to pause current Banuba Face AR effect i.e. some AR effects might play audio or
        // continue processing something in background. ".pause" method helps to stop processing.
        BanubaExtensionManager.pause()
    }

    override fun onDestroy() {
        super.onDestroy()
        // Destroys Banuba Face AR surface
        BanubaExtensionManager.loadEffect("")
        BanubaExtensionManager.release()
        enableBanubaExtension(false)
        RtcEngine.destroy()
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<String>,
        results: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, results)

        startRtcAndExtensionOr {
            showToast("Please grant permission and try again")
            finish()
        }
    }

    private fun startRtcAndExtensionOr(deniedBlock: () -> Unit ) {
        if (checkAllPermissionsGranted()) {
            startPreviewAndExtension()
        } else {
            deniedBlock()
        }
    }

    private fun checkAllPermissionsGranted() = REQUIRED_PERMISSIONS.all {
        ContextCompat.checkSelfPermission(baseContext, it) == PackageManager.PERMISSION_GRANTED
    }

    private fun startPreviewAndExtension() {
        Log.d(TAG, "Start local preview")
        // Important - extension works only after start preview
        addLocalVideo()
        agoraRtc.startPreview()

        BanubaExtensionManager.initialize(
            applicationContext,
            BANUBA_LICENSE_TOKEN,
            agoraRtc
        )
        BanubaExtensionManager.enableMirroring(isFrontCamera)
    }

    private fun joinChannel(channel: String) {
        if (!isJoinedToChannel) {
            Log.d(TAG, "Join to Agora channel = $channel")
            isJoinedToChannel = true
            agoraRtc.joinChannel(AGORA_TOKEN, channel, null, 0)

            invalidateUiState()
        }
    }

    private fun leaveChannel() {
        if (isJoinedToChannel) {
            Log.d(TAG, "Leave Agora channel")

            isJoinedToChannel = false
            agoraRtc.leaveChannel()

            removeRemoteVideo()
            invalidateUiState()
        }
    }

    private fun enableBanubaExtension(enable: Boolean) {
            agoraRtc.enableExtension(
                BANUBA_PROVIDER_NAME,
                BANUBA_EXTENSION_NAME,
                enable,
                Constants.MediaSourceType.PRIMARY_CAMERA_SOURCE
            )
    }

    private fun addLocalVideo() {
        val view = SurfaceView(this).apply { setZOrderMediaOverlay(true) }
        agoraRtc.setupLocalVideo(
            VideoCanvas(
                view,
                VideoCanvas.RENDER_MODE_HIDDEN,
                UID_LOCAL
            )
        )
        agoraRtc.setLocalRenderMode(
            Constants.RENDER_MODE_HIDDEN,
            Constants.VIDEO_MIRROR_MODE_DISABLED
        )
        binding.localVideoContainer.removeAllViews()
        binding.localVideoContainer.addView(view)
    }

    private fun removeRemoteVideo() {
        binding.remoteVideoContainer.removeAllViews()
    }

    private fun addRemoteVideo(uid: Int) {
        val view = SurfaceView(this)
        agoraRtc.setupRemoteVideo(VideoCanvas(view, VideoCanvas.RENDER_MODE_HIDDEN, uid))
        binding.remoteVideoContainer.addView(view)
    }

    /**
     * SAMPLE SPECIFIC CODE. NOT REQUIRED FOR INTEGRATION
     */
    private fun toggleEffect() {
        effectIndex++
        if (effectIndex >= availableEffects.size) {
            effectIndex = -1
        }
        invalidateUiState()
    }

    private fun showToast(message: String) = Toast.makeText(
        applicationContext, message, Toast.LENGTH_SHORT
    ).show()

    private fun invalidateUiState() {
        val nextEffectIndex = effectIndex + 1
        binding.applyEffectButton.text = if (nextEffectIndex >= availableEffects.size) {
            "Cancel effect"
        } else {
            "Apply ${availableEffects[nextEffectIndex]}"
        }

        if (isJoinedToChannel) {
            binding.joinButton.visibility = View.GONE
            binding.leaveButton.visibility = View.VISIBLE
        } else {
            binding.joinButton.visibility = View.VISIBLE
            binding.leaveButton.visibility = View.GONE
        }
    }

    private fun getCurrentEffect(): String = if (effectIndex == -1) {
        ""
    } else {
        availableEffects[effectIndex]
    }
}
