package com.sheraz.oboerecorder

class AudioEngine {

    companion object {

        init {
            System.loadLibrary("audioEngine")
        }

        // Native methods
        @JvmStatic external fun create(): Boolean
        @JvmStatic external fun delete()

        @JvmStatic external fun startRecording()
        @JvmStatic external fun stopRecording()

        @JvmStatic external fun startPlayingRecordedStream()
        @JvmStatic external fun stopPlayingRecordedStream()

        @JvmStatic external fun writeFile(filePath: String)

        @JvmStatic external fun startPlayingFromFile(filePath: String)
        @JvmStatic external fun stopPlayingFromFile()
    }
}