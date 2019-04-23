
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

void AudioEngine::startPlayingRecordedStream() {

    LOGD(TAG, "startPlayingRecordedStream(): ");

    openPlaybackStreamFromRecordedStreamParameters();

    if (mPlaybackStream) {
        startStream(mPlaybackStream);
    } else {
        LOGE(TAG, "startPlayingRecordedStream(): Failed to create recording (%p) stream", mRecordingStream);
        closeStream(mPlaybackStream);
    }

}

void AudioEngine::stopPlayingRecordedStream() {

    LOGD(TAG, "stopPlayingRecordedStream(): ");

    stopStream(mPlaybackStream);
    closeStream(mPlaybackStream);
    mSoundRecording.setReadPositionToStart();

}

void AudioEngine::startPlayingFromFile(const char* filePath) {

    LOGD(TAG, "startPlayingFromFile(): ");

    sndfileHandle = SndfileHandle(filePath);

    openPlaybackStreamFromFileParameters();

    if (mPlaybackStream) {
        startStream(mPlaybackStream);
    } else {
        LOGE(TAG, "startPlayingFromFile(): Failed to create recording (%p) stream", mRecordingStream);
        closeStream(mPlaybackStream);
    }


}

void AudioEngine::stopPlayingFromFile() {

    LOGD(TAG, "stopPlayingFromFile(): ");

    stopStream(mPlaybackStream);
    closeStream(mPlaybackStream);

}

void AudioEngine::writeToFile(const char* filePath) {

    LOGD(TAG, "writeToFile(): ");

//    mSoundRecording.initiateWritingToFile(filePath, mOutputChannelCount, static_cast<int32_t>(mSampleRate * 0.465));
    mSoundRecording.initiateWritingToFile(filePath, mOutputChannelCount, mSampleRate);

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

void AudioEngine::openPlaybackStreamFromRecordedStreamParameters() {

    LOGD(TAG, "openPlaybackStreamFromRecordedStreamParameters(): ");

    oboe::AudioStreamBuilder builder;

    setUpPlaybackStreamParameters(&builder, mAudioApi, mFormat, &playingCallback,
            mPlaybackDeviceId, mSampleRate, mOutputChannelCount);

    playingCallback.setPlaybackFromFile(false);

    oboe::Result result = builder.openStream(&mPlaybackStream);
    if (result == oboe::Result::OK && mPlaybackStream) {
        assert(mPlaybackStream->getChannelCount() == mOutputChannelCount);
//        assert(mPlaybackStream->getSampleRate() == mSampleRate);
        assert(mPlaybackStream->getFormat() == mFormat);

        mSampleRate = mPlaybackStream->getSampleRate();
        LOGV(TAG, "openPlaybackStreamFromRecordedStreamParameters(): mSampleRate = ");
        LOGV(TAG, std::to_string(mSampleRate).c_str());

        mFramesPerBurst = mPlaybackStream->getFramesPerBurst();
        LOGV(TAG, "openPlaybackStreamFromRecordedStreamParameters(): mFramesPerBurst = ");
        LOGV(TAG, std::to_string(mFramesPerBurst).c_str());

        // Set the buffer size to the burst size - this will give us the minimum possible latency
        mPlaybackStream->setBufferSizeInFrames(mFramesPerBurst);

    } else {
        LOGE(TAG, "openPlaybackStreamFromRecordedStreamParameters(): Failed to create recording stream. Error: %s",
             oboe::convertToText(result));
    }

}

void AudioEngine::openPlaybackStreamFromFileParameters() {

    LOGD(TAG, "openPlaybackStreamFromFileParameters(): ");

    oboe::AudioStreamBuilder builder;

    mSampleRate = sndfileHandle.samplerate();

    int audioFileFormat = sndfileHandle.format();
    LOGD(TAG, "openPlaybackStreamFromFileParameters(): audioFileFormat = ");
    LOGD(TAG, std::to_string(audioFileFormat).c_str());

    mOutputChannelCount = sndfileHandle.channels();

    setUpPlaybackStreamParameters(&builder, mAudioApi, mFormat, &playingCallback,
                                  mPlaybackDeviceId, mSampleRate, mOutputChannelCount);

    playingCallback.setPlaybackFromFile(true);

    oboe::Result result = builder.openStream(&mPlaybackStream);
    if (result == oboe::Result::OK && mPlaybackStream) {
        assert(mPlaybackStream->getChannelCount() == mOutputChannelCount);
//        assert(mPlaybackStream->getSampleRate() == mSampleRate);
        assert(mPlaybackStream->getFormat() == mFormat);

        mSampleRate = mPlaybackStream->getSampleRate();
        LOGV(TAG, "openPlaybackStreamFromFileParameters(): mSampleRate = ");
        LOGV(TAG, std::to_string(mSampleRate).c_str());

        mFramesPerBurst = mPlaybackStream->getFramesPerBurst();
        LOGV(TAG, "openPlaybackStreamFromFileParameters(): mFramesPerBurst = ");
        LOGV(TAG, std::to_string(mFramesPerBurst).c_str());

        // Set the buffer size to the burst size - this will give us the minimum possible latency
        mPlaybackStream->setBufferSizeInFrames(mFramesPerBurst);

    } else {
        LOGE(TAG, "openPlaybackStreamFromFileParameters(): Failed to create recording stream. Error: %s",
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

oboe::AudioStreamBuilder *AudioEngine::setUpPlaybackStreamParameters(oboe::AudioStreamBuilder *builder,
        oboe::AudioApi audioApi, oboe::AudioFormat audioFormat, oboe::AudioStreamCallback *audioStreamCallback,
        int32_t deviceId, int32_t sampleRate, int channelCount) {

    LOGD(TAG, "setUpPlaybackStreamParameters(): ");

    builder->setAudioApi(audioApi)
            ->setFormat(audioFormat)
            ->setSharingMode(oboe::SharingMode::Exclusive)
            ->setPerformanceMode(oboe::PerformanceMode::LowLatency)
            ->setCallback(audioStreamCallback)
            ->setDeviceId(deviceId)
            ->setDirection(oboe::Direction::Output)
            ->setSampleRate(sampleRate)
            ->setChannelCount(channelCount);
    return builder;

}

