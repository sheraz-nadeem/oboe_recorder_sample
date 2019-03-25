
#include <jni.h>
#include "AudioEngine.h"
#include "logging_macros.h"

const char *TAG = "jni_bridge";
static AudioEngine *audioEngine = nullptr;

extern "C" {

JNIEXPORT bool JNICALL
Java_com_sheraz_oboerecorder_AudioEngine_create(JNIEnv *env, jclass) {

    LOGD(TAG, "create(): ");

    if (audioEngine == nullptr) {
        audioEngine = new AudioEngine();
    }

    return (audioEngine != nullptr);
}

JNIEXPORT void JNICALL
Java_com_sheraz_oboerecorder_AudioEngine_delete(JNIEnv *env, jclass) {

    LOGD(TAG, "delete(): ");

    delete audioEngine;
    audioEngine = nullptr;

}

JNIEXPORT void JNICALL
Java_com_sheraz_oboerecorder_AudioEngine_startRecording(JNIEnv *env, jclass) {

    LOGD(TAG, "startRecording(): ");

    if (audioEngine == nullptr) {
        LOGE(TAG, "audioEngine is null, you must call create() method before calling this method");
        return;
    }

    audioEngine->startRecording();

}

JNIEXPORT void JNICALL
Java_com_sheraz_oboerecorder_AudioEngine_stopRecording(JNIEnv *env, jclass) {

    LOGD(TAG, "stopRecording(): ");

    if (audioEngine == nullptr) {
        LOGE(TAG, "audioEngine is null, you must call create() method before calling this method");
        return;
    }

    audioEngine->stopRecording();

}
};

