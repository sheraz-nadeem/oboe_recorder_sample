#include "SoundRecording.h"


#include <android/log.h>
#include <string>
#include "SoundRecording.h"
#include "logging_macros.h"

int32_t SoundRecording::write(const float *sourceData, int32_t numSamples) {

    LOGD(TAG, "write(): ");

    // Check that data will fit, if it doesn't just write as much as we can.
    if (mWriteIndex + numSamples > kMaxSamples) {

        LOGW(TAG, "write(): mWriteIndex + numSamples > kMaxSamples");
        numSamples = kMaxSamples - mWriteIndex;

        mIteration++;
        int32_t newSize = kMaxSamples*mIteration;

        auto * newData = new float[newSize] { 0 };
        std::copy(mData, mData + mWriteIndex, newData);

        mDataPointer = newData;
        mWriteIndex = 0;
        mData = new float[kMaxSamples] { 0 };

    }

    for (int i = 0; i < numSamples; ++i) {
        mData[mWriteIndex++] = sourceData[i];
    }

    mTotalSamples += numSamples;

    return numSamples;
}

int32_t SoundRecording::read(float *targetData, int32_t numSamples) {

    LOGD(TAG, "read(): ");

    int32_t framesRead = 0;
    while (framesRead < numSamples && mReadIndex < mTotalSamples) {
        targetData[framesRead++] = mDataPointer[mReadIndex++];
        if (mIsLooping && mReadIndex == mTotalSamples) mReadIndex = 0;
    }
    return framesRead;
}