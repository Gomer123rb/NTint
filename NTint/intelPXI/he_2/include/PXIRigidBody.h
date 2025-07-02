#pragma once
#include "PXIVec3.h"

class PXIRigidBody {
public:
    PXIVec3 position;
    PXIVec3 velocity;
    float mass;

    PXIRigidBody(PXIVec3 pos, float m)
        : position(pos), velocity(0,0,0), mass(m) {}

    void applyForce(const PXIVec3& force);
    void update(float deltaTime);
};
