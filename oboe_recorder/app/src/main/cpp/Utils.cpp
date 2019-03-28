#include <cstring>
#include "Utils.h"


void fillArrayWithZeros(float *data, int32_t length) {
    memset(data, 0, length * sizeof(float));
}