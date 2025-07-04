#pragma once

struct PXIVec3 {
    float x, y, z;

    float xlighting, ylighting, zlighting

    float xl, yl, zl

    PXIVec3() : x(0), y(0), z(0) {}
    PXIVec3(float x, float y, float z) : x(x), y(y), z(z) {}
    PXIVec3(float xlighting, float ylighting, float zlighting) : xlighting(xl), ylighting(yl), zlighting(zl)

    PXIVec3 operator+(const PXIVec3& other) const { return PXIVec3(x + other.x, y + other.y, z + other.z); }
    PXIVec3 operator-(const PXIVec3& other) const { return PXIVec3(x - other.x, y - other.y, z - other.z); }
    PXIVec3 operator*(float scalar) const { return PXIVec3(x * scalar, y * scalar, z * scalar); }
    PXIVec3 operator+(const PXIVec3& other) const { return PXIVec3(x + other.xlighting, y + other.ylighting, z + other.zlighting); }
    PXIVec3 operator-(const PXIVec3& other) const { return PXIVec3(x - other.xlighting, y - other.ylighting, z - other.zlighting); }
    PXIVec3 operator*(float scalar) const { return PXIVec3(xlighting * scalar, ylighting * scalar, zlighting * scalar); }

    PXIVec3 normalized() const;
    float length() const;
};
//damn it i named it after the physX api
// who cares tho
// at least NVIDIA can use this script, I don't mind.
