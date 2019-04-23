#ifndef OBOE_RECORDER_PLAYINGCALLBACK_H
#define OBOE_RECORDER_PLAYINGCALLBACK_H


#include <oboe/Definitions.h>
#include <oboe/AudioStream.h>
#include "SoundRecording.h"
#include "logging_macros.h"

#ifndef MODULE_NAME
#define MODULE_NAME  "PlayingCallback"
#endif


class PlayingCallback : public oboe::AudioStreamCallback {

private:
    const char* TAG = "PlayingCallback:: %s";
    SoundRecording* mSoundRecording = nullptr;
    SndfileHandle* mFileHandle = nullptr;
    bool isPlaybackFromFile = false;

public:
    PlayingCallback() = default;

    explicit PlayingCallback(SoundRecording* recording, SndfileHandle* handle) {
        mSoundRecording = recording;
        mFileHandle = handle;
    }

    bool isPlayingFromFile() { return isPlaybackFromFile; }
    void setPlaybackFromFile(bool isFile) { isPlaybackFromFile = isFile; }

    oboe::DataCallbackResult
    onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames);

    oboe::DataCallbackResult
    processPlaybackFrames(oboe::AudioStream *audioStream, int16_t *audioData, int32_t numFrames);

};

#endif //OBOE_RECORDER_PLAYINGCALLBACK_H
