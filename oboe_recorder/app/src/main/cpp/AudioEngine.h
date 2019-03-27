
#ifndef OBOE_RECORDER_AUDIOENGINE_H
#define OBOE_RECORDER_AUDIOENGINE_H

#ifndef MODULE_NAME
#define MODULE_NAME  "AudioEngine"
#endif

#include <oboe/Definitions.h>
#include <oboe/AudioStream.h>
#include "SoundRecording.h"
#include "logging_macros.h"

class RecordingCallback : public oboe::AudioStreamCallback {

private:
    const char* TAG = "RecordingCallback:: %s";
    SoundRecording* mSoundRecording = nullptr;

public:
    RecordingCallback() = default;

    explicit RecordingCallback(SoundRecording* recording) {
        mSoundRecording = recording;
    }

    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) override {

        LOGD(TAG, "onAudioReady(): ");
        LOGI(TAG, std::to_string(numFrames).c_str());

        int32_t framesWritten = mSoundRecording->write(static_cast<float *>(audioData), numFrames);
        LOGI(TAG, std::to_string(framesWritten).c_str());

        return oboe::DataCallbackResult::Continue;
    }
};


class PlayingCallback : public oboe::AudioStreamCallback {

private:
    const char* TAG = "PlayingCallback:: %s";
    SoundRecording* mSoundRecording = nullptr;

public:
    PlayingCallback() = default;

    explicit PlayingCallback(SoundRecording* recording) {
        mSoundRecording = recording;
    }

    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) override {

        LOGD(TAG, "onAudioReady(): ");
        LOGI(TAG, std::to_string(numFrames).c_str());

        int32_t framesWritten = mSoundRecording->read(static_cast<float *>(audioData), numFrames);
        LOGI(TAG, std::to_string(framesWritten).c_str());

        return oboe::DataCallbackResult::Continue;
    }
};

class AudioEngine {

public:
    AudioEngine();
    ~AudioEngine();

    RecordingCallback recordingCallback = RecordingCallback(&mSoundRecording);
    PlayingCallback playingCallback = PlayingCallback(&mSoundRecording);

    void startRecording();
    void stopRecording();
    void startPlaying();
    void stopPlaying();

//    oboe::DataCallbackResult onAudioReady(oboe::AudioStream *oboeStream, void *audioData, int32_t numFrames) override;


private:

    const char* TAG = "AudioEngine:: %s";

    int32_t mRecordingDeviceId = oboe::VoiceCommunication;
    int32_t mPlaybackDeviceId = oboe::kUnspecified;

    oboe::AudioFormat mFormat = oboe::AudioFormat::I16;
    int32_t mSampleRate = oboe::kUnspecified;
    int32_t mInputChannelCount = oboe::ChannelCount::Stereo;
    int32_t mOutputChannelCount = oboe::ChannelCount::Stereo;

    oboe::AudioApi mAudioApi = oboe::AudioApi::AAudio;
    oboe::AudioStream *mRecordingStream = nullptr;
    oboe::AudioStream *mPlaybackStream = nullptr;
    SoundRecording mSoundRecording;

    void openRecordingStream();
    void openPlaybackStream();

    void startStream(oboe::AudioStream *stream);
    void stopStream(oboe::AudioStream *stream);
    void closeStream(oboe::AudioStream *stream);

    oboe::AudioStreamBuilder* setUpRecordingStreamParameters(oboe::AudioStreamBuilder* builder);
    oboe::AudioStreamBuilder* setUpPlaybackStreamParameters(oboe::AudioStreamBuilder* builder);

};


#endif //OBOE_RECORDER_AUDIOENGINE_H
