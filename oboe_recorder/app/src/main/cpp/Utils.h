
#ifndef OBOE_RECORDER_UTILS_H
#define OBOE_RECORDER_UTILS_H

#ifndef MODULE_NAME
#define MODULE_NAME  "Utils"
#endif

#include <cstdint>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
#include <ios>
#include "logging_macros.h"


template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 64)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

template <typename K>
void fillArrayWithZeros(K *data, int32_t length) {

    size_t bufferSize = length * sizeof(K);
    LOGI("Utils:: %s", "fillArrayWithZeros(): bufferSize = ");
    LOGI("Utils:: %s", to_string_with_precision(bufferSize).c_str());

    memset(data, 0, bufferSize);
}

#endif //OBOE_RECORDER_UTILS_H
