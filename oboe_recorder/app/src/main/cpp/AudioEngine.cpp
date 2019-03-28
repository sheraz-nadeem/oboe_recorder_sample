
#include <oboe/Oboe.h>
#include "AudioEngine.h"
#include "logging_macros.h"



AudioEngine::AudioEngine() {
    assert(mOutputChannelCount == mInputChannelCount);
}

AudioEngine::~AudioEngine() {
    stopStream(mRecordingStream);
    closeStream(mRecordingStream);

    stopStream(mPlaybackStream);
    closeStream(mPlaybackStream);
}

void AudioEngine::startRecording() {

    LOGD(TAG, "startRecording(): ");

    openRecordingStream();

    if (mRecordingStream) {
        startStream(mRecordingStream);
    } else {
        LOGE(TAG, "startRecording(): Failed to create recording (%p) stream", mRecordingStream);
        closeStream(mRecordingStream);
    }

}

void AudioEngine::stopRecording() {

    LOGD(TAG, "stopRecording(): ");

    stopStream(mRecordingStream);
    closeStream(mRecordingStream);

}

void AudioEngine::startPlaying() {

    LOGD(TAG, "startPlaying(): ");

    openPlaybackStream();

    if (mPlaybackStream) {
        startStream(mPlaybackStream);
    } else {
        LOGE(TAG, "startPlaying(): Failed to create recording (%p) stream", mRecordingStream);
        closeStream(mPlaybackStream);
    }

}

void AudioEngine::stopPlaying() {

    LOGD(TAG, "stopPlaying(): ");

    stopStream(mPlaybackStream);
    closeStream(mPlaybackStream);
    mSoundRecording.setReadPositionToStart();

}

void AudioEngine::openRecordingStream() {

    LOGD(TAG, "openRecordingStream(): ");

    oboe::AudioStreamBuilder builder;

    setUpRecordingStreamParameters(&builder);

    oboe::Result result = builder.openStream(&mRecordingStream);
    if (result == oboe::Result::OK && mRecordingStream) {
        assert(mRecordingStream->getChannelCount() == mInputChannelCount);
//        assert(mRecordingStream->getSampleRate() == mSampleRate);
//        assert(mRecordingStream->getFormat() == mFormat);

        mSampleRate = mRecordingStream->getSampleRate();
        mFormat = mRecordingStream->getFormat();
        LOGV(TAG, "openRecordingStream(): mSampleRate = ");
        LOGV(TAG, std::to_string(mSampleRate).c_str());

        LOGV(TAG, "openRecordingStream(): mFormat = ");
        LOGV(TAG, oboe::convertToText(mFormat));

    } else {
        LOGE(TAG, "Failed to create recording stream. Error: %s",
             oboe::convertToText(result));
    }

}

void AudioEngine::openPlaybackStream() {

    LOGD(TAG, "openPlaybackStream(): ");

    oboe::AudioStreamBuilder builder;

    setUpPlaybackStreamParameters(&builder);

    oboe::Result result = builder.openStream(&mPlaybackStream);
    if (result == oboe::Result::OK && mPlaybackStream) {
        assert(mPlaybackStream->getChannelCount() == mOutputChannelCount);
//        assert(mPlaybackStream->getSampleRate() == mSampleRate);
        assert(mPlaybackStream->getFormat() == mFormat);

        mSampleRate = mPlaybackStream->getSampleRate();
        LOGV(TAG, "openPlaybackStream(): mSampleRate = ");
        LOGV(TAG, std::to_string(mSampleRate).c_str());

    } else {
        LOGE(TAG, "openPlaybackStream(): Failed to create recording stream. Error: %s",
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
        LOGW(TAG, "stopStream(): mTotalSamples = ");
        LOGW(TAG, std::to_string(mSoundRecording.getTotalSamples()).c_str());
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

        LOGW(TAG, "closeStream(): mTotalSamples = ");
        LOGW(TAG, std::to_string(mSoundRecording.getTotalSamples()).c_str());
    }

}

oboe::AudioStreamBuilder *AudioEngine::setUpRecordingStreamParameters(oboe::AudioStreamBuilder *builder) {

    LOGD(TAG, "setUpRecordingStreamParameters(): ");

    builder->setAudioApi(mAudioApi)
            ->setFormat(mFormat)
            ->setSharingMode(oboe::SharingMode::Exclusive)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setCallback(&recordingCallback)
            ->setDeviceId(mRecordingDeviceId)
            ->setDirection(oboe::Direction::Input)
//            ->setSampleRate(mSampleRate)
            ->setChannelCount(mInputChannelCount);
    return builder;
}

oboe::AudioStreamBuilder *AudioEngine::setUpPlaybackStreamParameters(oboe::AudioStreamBuilder *builder) {

    LOGD(TAG, "setUpPlaybackStreamParameters(): ");

    builder->setAudioApi(mAudioApi)
            ->setFormat(mFormat)
            ->setSharingMode(oboe::SharingMode::Exclusive)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setCallback(&playingCallback)
            ->setDeviceId(mPlaybackDeviceId)
            ->setDirection(oboe::Direction::Output)
            ->setSampleRate(mSampleRate)
            ->setChannelCount(mOutputChannelCount);
    return builder;

}



//oboe::DataCallbackResult AudioEngine::onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) {
//
//    LOGD(TAG, "onAudioReady(): ");
//
//    assert(oboeStream == mRecordingStream);
//
//    LOGI(TAG, std::to_string(numFrames).c_str());
//
//    int32_t framesWritten = mSoundRecording.write(static_cast<float *>(audioData), numFrames);
//    LOGI(TAG, std::to_string(framesWritten).c_str());
//
//    return oboe::DataCallbackResult::Continue;
//
//}
