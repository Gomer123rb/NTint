#pragma once

struct PXIVec3 {
    float x, y, z;
    float xlighting, ylighting, zlighting;
    float xl, yl, zl;

    // Default constructor
    PXIVec3()
        : x(0), y(0), z(0),
          xlighting(0), ylighting(0), zlighting(0),
          xl(0), yl(0), zl(0) {}

    // Position constructor
    PXIVec3(float X, float Y, float Z)
        : x(X), y(Y), z(Z),
          xlighting(0), ylighting(0), zlighting(0),
          xl(0), yl(0), zl(0) {}

    // Lighting constructor
    PXIVec3(float XL, float YL, float ZL, bool lighting)
        : x(0), y(0), z(0),
          xlighting(XL), ylighting(YL), zlighting(ZL),
          xl(XL), yl(YL), zl(ZL) {}

    // Basic arithmetic (position)
    PXIVec3 operator+(const PXIVec3& other) const {
        return PXIVec3(x + other.x, y + other.y, z + other.z);
    }

    PXIVec3 operator-(const PXIVec3& other) const {
        return PXIVec3(x - other.x, y - other.y, z - other.z);
    }

    PXIVec3 operator*(float scalar) const {
        return PXIVec3(x * scalar, y * scalar, z * scalar);
    }

    // Lighting arithmetic
    PXIVec3 addLighting(const PXIVec3& other) const {
        return PXIVec3(xlighting + other.xlighting,
                       ylighting + other.ylighting,
                       zlighting + other.zlighting,
                       true);
    }

    PXIVec3 mulLighting(float scalar) const {
        return PXIVec3(xlighting * scalar,
                       ylighting * scalar,
                       zlighting * scalar,
                       true);
    }

    float length() const;
    PXIVec3 normalized() const;
};
//damn it i named it after the physX api 
// who cares tho 
// at least NVIDIA can use this script, I don't mind.