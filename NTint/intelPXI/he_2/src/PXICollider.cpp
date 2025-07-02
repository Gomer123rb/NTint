#include "PXICollider.h"
#include <cmath>

bool AABB::intersects(const AABB& other) const {
    return (min.x <= other.max.x && max.x >= other.min.x) &&
           (min.y <= other.max.y && max.y >= other.min.y) &&
           (min.z <= other.max.z && max.z >= other.min.z);
}

bool Sphere::intersects(const Sphere& other) const {
    float dist = (center - other.center).length();
    return dist < (radius + other.radius);
}
