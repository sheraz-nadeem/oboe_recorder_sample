
#ifndef OBOE_RECORDER_AUDIOENGINE_H
#define OBOE_RECORDER_AUDIOENGINE_H

#ifndef MODULE_NAME
#define MODULE_NAME  "AudioEngine"
#endif

#include <oboe/Definitions.h>
#include <oboe/AudioStream.h>
#include "SoundRecording.h"
#include "logging_macros.h"
#include "Utils.h"
#include "RecordingCallback.h"
#include "PlayingCallback.h"
#include <sndfile.h>


class AudioEngine {

public:
    AudioEngine();
    ~AudioEngine();

    RecordingCallback recordingCallback = RecordingCallback(&mSoundRecording);
    PlayingCallback playingCallback = PlayingCallback(&mSoundRecording, &sndfileHandle);

    void startRecording();
    void stopRecording();
    void startPlayingRecordedStream();
    void stopPlayingRecordedStream();
    void startPlayingFromFile(const char* filePath);
    void stopPlayingFromFile();
    void writeToFile(const char* filePath);

private:

    const char* TAG = "AudioEngine:: %s";

    int32_t mRecordingDeviceId = oboe::VoiceRecognition;
//    int32_t mPlaybackDeviceId = oboe::kUnspecified;
    int32_t mPlaybackDeviceId = 6;

    oboe::AudioFormat mFormat = oboe::AudioFormat::I16;
    int32_t mSampleRate = oboe::kUnspecified;
    int32_t mFramesPerBurst;
    int32_t mInputChannelCount = oboe::ChannelCount::Stereo;
    int32_t mOutputChannelCount = oboe::ChannelCount::Stereo;

    oboe::AudioApi mAudioApi = oboe::AudioApi::AAudio;
    oboe::AudioStream *mRecordingStream = nullptr;
    oboe::AudioStream *mPlaybackStream = nullptr;
    SoundRecording mSoundRecording;
    SndfileHandle sndfileHandle;

    void openRecordingStream();
    void openPlaybackStreamFromRecordedStreamParameters();
    void openPlaybackStreamFromFileParameters();

    void startStream(oboe::AudioStream *stream);
    void stopStream(oboe::AudioStream *stream);
    void closeStream(oboe::AudioStream *stream);

    oboe::AudioStreamBuilder* setUpRecordingStreamParameters(oboe::AudioStreamBuilder* builder);
    oboe::AudioStreamBuilder* setUpPlaybackStreamParameters(oboe::AudioStreamBuilder *builder,
                                                            oboe::AudioApi audioApi, oboe::AudioFormat audioFormat, oboe::AudioStreamCallback *audioStreamCallback,
                                                            int32_t deviceId, int32_t sampleRate, int channelCount);

};


#endif //OBOE_RECORDER_AUDIOENGINE_H
