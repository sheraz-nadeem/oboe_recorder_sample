
#ifndef OBOE_RECORDER_SOUNDRECORDING_H
#define OBOE_RECORDER_SOUNDRECORDING_H

#ifndef MODULE_NAME
#define MODULE_NAME  "SoundRecording"
#endif

#include <cstdint>
#include <array>
#include <atomic>



constexpr int kMaxSamples = 480000; // 10s of audio data @ 48kHz
constexpr int kIterationMultiplier = 10;

class SoundRecording {

public:
    int32_t write(const float *sourceData, int32_t numSamples);
    int32_t read(float *targetData, int32_t numSamples);
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
//    std::array<float,kMaxSamples> mData { 0 };
    float* mData = new float[kMaxSamples]{0.0};
    float* mDataPointer = mData;
};

#endif //OBOE_RECORDER_SOUNDRECORDING_H
