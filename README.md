
OBOE RECORDER SAMPLE
======================================

This android application (Native/C++) is created to demonstrate how we can use [Google's Oboe](https://github.com/google/oboe) to record 
audio and saving them into a local WAV file.

Used
---------------
1. Used **Kotlin**.
2. Whole project is based on [AndroidX](https://developer.android.com/jetpack/androidx/).
3. Used Oboe (A C++ library that makes it easy to build high-performance audio apps on Android).
4. Used C library named [**libsndfile**](https://github.com/erikd/libsndfile) for reading and writing sound files containing sampled audio data.

TODOs
-----
1. Writing to file must be done on a background thread.
2. UI improvements, giving feedback to user while recording/playback etc etc.
3. Current implementation replaces old recorded file, we need to just create new recording in to a new file
each time user starts recording.
4. Give user the control to save recordings to a new directory location.

