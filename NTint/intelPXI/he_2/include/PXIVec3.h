#pragma once

struct PXIVec3 {
    float x, y, z;

    PXIVec3() : x(0), y(0), z(0) {}
    PXIVec3(float x, float y, float z) : x(x), y(y), z(z) {}

    PXIVec3 operator+(const PXIVec3& other) const { return PXIVec3(x + other.x, y + other.y, z + other.z); }
    PXIVec3 operator-(const PXIVec3& other) const { return PXIVec3(x - other.x, y - other.y, z - other.z); }
    PXIVec3 operator*(float scalar) const { return PXIVec3(x * scalar, y * scalar, z * scalar); }

    PXIVec3 normalized() const;
    float length() const;
};
