package com.sheraz.oboerecorder

import android.Manifest
import android.content.pm.PackageManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Toast
import androidx.core.app.ActivityCompat
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    private val AUDIO_RECORD_REQUEST = 12446

    override fun onCreate(savedInstanceState: Bundle?) {

        Log.d(TAG, "onCreate: ")

        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        checkRecordAudioPermission()
        AudioEngine.create()
        initUI()

    }

    override fun onDestroy() {

        Log.d(TAG, "onDestroy: ")
        super.onDestroy()
        AudioEngine.delete()

    }

    private fun initUI() {

        Log.d(TAG, "initUI: ")

        btnStartRecording.setOnClickListener {

            if (!checkRecordAudioPermission()) {
                return@setOnClickListener
            }

            Log.d(TAG, "btnStartRecording.onClick: ")
            AudioEngine.startRecording()
        }

        btnStopRecording.setOnClickListener {

            if (!checkRecordAudioPermission()) {
                return@setOnClickListener
            }

            Log.d(TAG, "btnStopRecording.onClick: ")
            AudioEngine.stopRecording()
        }

        btnStartPlaying.setOnClickListener {

            if (!checkRecordAudioPermission()) {
                return@setOnClickListener
            }

            Log.d(TAG, "btnStartPlaying.onClick: ")
            AudioEngine.startPlaying()
        }

        btnStopPlaying.setOnClickListener {

            if (!checkRecordAudioPermission()) {
                return@setOnClickListener
            }

            Log.d(TAG, "btnStopPlaying.onClick: ")
            AudioEngine.stopPlaying()
        }

    }
    private fun checkRecordAudioPermission(): Boolean {

        Log.d(TAG, "checkRecordAudioPermission: ")

        val isRecordingAllowed = isRecordPermissionGranted()
        Log.i(TAG, "checkRecordAudioPermission: $isRecordingAllowed")

        if (!isRecordingAllowed) {

            requestRecordPermission()
            disableControls()

        } else {

            enableControls()

        }

        return isRecordingAllowed
    }

    private fun isRecordPermissionGranted(): Boolean {

        val permissionStatus = ActivityCompat
            .checkSelfPermission(this,
                Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED

        Log.d(TAG, "isRecordPermissionGranted: $permissionStatus")

        return permissionStatus
    }

    private fun requestRecordPermission() {

        Log.d(TAG, "requestRecordPermission: ")
        ActivityCompat.requestPermissions(
            this,
            arrayOf(Manifest.permission.RECORD_AUDIO),
            AUDIO_RECORD_REQUEST
        )

    }

    override fun onRequestPermissionsResult(
        requestCode: Int, permissions: Array<String>,
        grantResults: IntArray
    ) {

        if (AUDIO_RECORD_REQUEST != requestCode) {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
            return
        }

        if (grantResults.size != 1 || grantResults[0] != PackageManager.PERMISSION_GRANTED) {

            Log.w(TAG, "requestRecordPermission: RECORD_AUDIO Permission not granted, show toast")

            Toast.makeText(
                applicationContext,
                getString(R.string.need_record_audio_permission),
                Toast.LENGTH_SHORT).show()

        } else {

            Log.i(TAG, "requestRecordPermission: RECORD_AUDIO Permission granted, continue with enableControls")
            enableControls()

        }
    }

    private fun enableControls() {

        Log.d(TAG, "enableControls: ")

        tvStatus.text = ""
        btnStartPlaying.isEnabled = true
        btnStartRecording.isEnabled = true
        btnStopPlaying.isEnabled = true
        btnStopRecording.isEnabled = true

    }

    private fun disableControls() {

        Log.d(TAG, "disableControls: ")

        tvStatus.text = getString(R.string.need_record_audio_permission)
        btnStartPlaying.isEnabled = false
        btnStartRecording.isEnabled = false
        btnStopPlaying.isEnabled = false
        btnStopRecording.isEnabled = false

    }

    companion object {
        private val TAG = MainActivity::class.java.simpleName
    }
}
