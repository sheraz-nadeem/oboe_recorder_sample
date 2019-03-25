
#include <oboe/Oboe.h>
#include "AudioEngine.h"
#include "logging_macros.h"



AudioEngine::AudioEngine() {
    assert(mOutputChannelCount == mInputChannelCount);
}

AudioEngine::~AudioEngine() {
    stopStream(mRecordingStream);
    closeStream(mRecordingStream);
}

void AudioEngine::startRecording() {

    LOGD(TAG, "startRecording(): ");

    openRecordingStream();

    if (mRecordingStream) {
        startStream(mRecordingStream);
    } else {
        LOGE(TAG, "Failed to create recording (%p) stream", mRecordingStream);
        closeStream(mRecordingStream);
    }

}

void AudioEngine::stopRecording() {

    LOGD(TAG, "stopRecording(): ");

    stopStream(mRecordingStream);
    closeStream(mRecordingStream);

}

void AudioEngine::openRecordingStream() {

    LOGD(TAG, "openRecordingStream(): ");

    oboe::AudioStreamBuilder builder;

    setUpRecordingStreamParameters(&builder);

    oboe::Result result = builder.openStream(&mRecordingStream);
    if (result == oboe::Result::OK && mRecordingStream) {
        assert(mRecordingStream->getChannelCount() == mInputChannelCount);
//        assert(mRecordingStream->getSampleRate() == mSampleRate);
        assert(mRecordingStream->getFormat() == oboe::AudioFormat::I16);

    } else {
        LOGE(TAG, "Failed to create recording stream. Error: %s",
             oboe::convertToText(result));
    }

}

void AudioEngine::startStream(oboe::AudioStream *stream) {

    LOGD(TAG, "startStream(): ");

    assert(stream);
    if (stream) {
        oboe::Result result = stream->requestStart();
        if (result != oboe::Result::OK) {
            LOGE(TAG, "Error starting stream. %s", oboe::convertToText(result));
        }
    }

}

void AudioEngine::stopStream(oboe::AudioStream *stream) {

    LOGD(TAG, "stopStream(): ");

    if (stream) {
        oboe::Result result = stream->stop(0L);
        if (result != oboe::Result::OK) {
            LOGE(TAG, "Error stopping stream. %s", oboe::convertToText(result));
        }
    }

}

void AudioEngine::closeStream(oboe::AudioStream *stream) {

    LOGD(TAG, "closeStream(): ");

    if (stream) {
        oboe::Result result = stream->close();
        if (result != oboe::Result::OK) {
            LOGE(TAG, "Error closing stream. %s", oboe::convertToText(result));
        } else {
            stream = nullptr;
        }
    }

}

oboe::AudioStreamBuilder *AudioEngine::setUpRecordingStreamParameters(oboe::AudioStreamBuilder *builder) {

    LOGD(TAG, "setUpRecordingStreamParameters(): ");

    builder->setAudioApi(mAudioApi)
            ->setFormat(mFormat)
            ->setSharingMode(oboe::SharingMode::Exclusive)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setCallback(nullptr)
            ->setDeviceId(mRecordingDeviceId)
            ->setDirection(oboe::Direction::Input)
//            ->setSampleRate(mSampleRate)
            ->setChannelCount(mInputChannelCount);
    return builder;
}

