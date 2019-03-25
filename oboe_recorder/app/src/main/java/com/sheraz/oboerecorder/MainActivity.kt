package com.sheraz.oboerecorder

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

//        // Example of a call to a native method
//        sample_text.text = stringFromJNI()

        AudioEngine.create()

        btnStartRecording.setOnClickListener {
            Log.d("MainActivity", "btnStartRecording.onClick: ")
            AudioEngine.startRecording()
        }

        btnStopRecording.setOnClickListener {
            Log.d("MainActivity", "btnStopRecording.onClick: ")
            AudioEngine.stopRecording()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        AudioEngine.delete()
    }

    //    /**
//     * A native method that is implemented by the 'native-lib' native library,
//     * which is packaged with this application.
//     */
//    external fun stringFromJNI(): String
//
//    companion object {
//
//        // Used to load the 'native-lib' library on application startup.
//        init {
//            System.loadLibrary("native-lib")
//        }
//    }
}
