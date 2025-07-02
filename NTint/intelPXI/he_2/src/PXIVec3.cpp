#include "../include/PXIVec3.h"
#include <cmath>

PXIVec3 PXIVec3::normalized() const {
    float len = length();
    return len != 0 ? (*this) * (1.0f / len) : PXIVec3();
}

float PXIVec3::length() const {
    return std::sqrt(x * x + y * y + z * z);
}