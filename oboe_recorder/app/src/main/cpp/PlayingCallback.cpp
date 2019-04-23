
#include "PlayingCallback.h"


oboe::DataCallbackResult
PlayingCallback::onAudioReady(oboe::AudioStream *audioStream, void *audioData, int32_t numFrames) {
    return processPlaybackFrames(audioStream, static_cast<int16_t *>(audioData), numFrames * audioStream->getChannelCount());
}

oboe::DataCallbackResult
PlayingCallback::processPlaybackFrames(oboe::AudioStream *audioStream, int16_t *audioData, int32_t numFrames) {

    LOGD(TAG, "processPlaybackFrames(): ");

    fillArrayWithZeros(audioData, numFrames);

    int64_t framesWritten = 0;

    if (!isPlayingFromFile()) {
        framesWritten = mSoundRecording->read(audioData, numFrames);
    } else {
        framesWritten = static_cast<sf_count_t>(mFileHandle->read(audioData, numFrames));
    }

    if (framesWritten == 0) {

        LOGW(TAG, "processPlaybackFrames(): framesWritten == 0, playback stream ended, stop playback.");
        audioStream->requestStop();

        return oboe::DataCallbackResult::Stop;

    }

    return oboe::DataCallbackResult::Continue;

}

