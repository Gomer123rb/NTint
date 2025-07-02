#pragma once
#include "PXIVec3.h"

struct AABB {
    PXIVec3 min;
    PXIVec3 max;

    bool intersects(const AABB& other) const;
};

struct Sphere {
    PXIVec3 center;
    float radius;

    bool intersects(const Sphere& other) const;
};
