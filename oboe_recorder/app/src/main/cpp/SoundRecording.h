
#ifndef OBOE_RECORDER_SOUNDRECORDING_H
#define OBOE_RECORDER_SOUNDRECORDING_H

#ifndef MODULE_NAME
#define MODULE_NAME  "SoundRecording"
#endif


#include <cstdint>
#include <array>
#include <atomic>
#include <sndfile.hh>
#include <oboe/Definitions.h>
#include <android/log.h>
#include <string>
#include <ios>
#include <sstream>
#include "SoundRecording.h"
#include "logging_macros.h"
#include "Utils.h"


constexpr int kMaxSamples = 480000; // 10s of audio data @ 48kHz

class SoundRecording {

public:
    int32_t write(const int16_t *sourceData, int32_t numSamples);
    int32_t read(int16_t *targetData, int32_t numSamples);

    void initiateWritingToFile(const char *outfilename, int32_t outputChannels, int32_t sampleRate);
    SndfileHandle createFile(const char *outfilename, int32_t outputChannels, int32_t sampleRate);
    void writeFile(SndfileHandle sndfileHandle);
    void readFileInfo(const char * fileName);

    bool isFull() const { return (mWriteIndex == kMaxSamples); };
    void setReadPositionToStart() { mReadIndex = 0; };
    void clear() { mWriteIndex = 0; };
    void setLooping(bool isLooping) { mIsLooping = isLooping; };
    int32_t getLength() const { return mWriteIndex; };
    int32_t getTotalSamples() const { return mTotalSamples; };
    static const int32_t getMaxSamples() { return kMaxSamples; };

private:
    const char* TAG = "SoundRecording:: %s";

    std::atomic<int32_t> mIteration { 1 };
    std::atomic<int32_t> mWriteIndex { 0 };
    std::atomic<int32_t> mReadIndex { 0 };
    std::atomic<int32_t> mTotalSamples { 0 };
    std::atomic<bool> mIsLooping { false };

    int16_t* mData = new int16_t[kMaxSamples]{0};

    // 6 Decibels gain on audio signal
    int16_t gain_factor = 2;
};

#endif //OBOE_RECORDER_SOUNDRECORDING_H
