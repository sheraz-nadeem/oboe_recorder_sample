
#ifndef OBOE_RECORDER_AUDIOENGINE_H
#define OBOE_RECORDER_AUDIOENGINE_H


#include <oboe/Definitions.h>
#include <oboe/AudioStream.h>

class AudioEngine {

public:
    AudioEngine();
    ~AudioEngine();

    void startRecording();
    void stopRecording();


private:

    const char* TAG = "AudioEngine";

    int32_t mRecordingDeviceId = oboe::VoiceCommunication;

    oboe::AudioFormat mFormat = oboe::AudioFormat::I16;
//    int32_t mSampleRate = oboe::kUnspecified;
    int32_t mInputChannelCount = oboe::ChannelCount::Stereo;
    int32_t mOutputChannelCount = oboe::ChannelCount::Stereo;

    oboe::AudioApi mAudioApi = oboe::AudioApi::AAudio;
    oboe::AudioStream *mRecordingStream = nullptr;

    void openRecordingStream();

    void startStream(oboe::AudioStream *stream);
    void stopStream(oboe::AudioStream *stream);
    void closeStream(oboe::AudioStream *stream);

    oboe::AudioStreamBuilder* setUpRecordingStreamParameters(oboe::AudioStreamBuilder* builder);

};




#endif //OBOE_RECORDER_AUDIOENGINE_H
