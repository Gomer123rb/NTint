#pragma once
#include <vector>
#include "PXIRigidBody.h"

class PXIPhysicsWorld {
public:
    PXIVec3 gravity;
    std::vector<PXIRigidBody*> bodies;

    PXIPhysicsWorld(PXIVec3 g = PXIVec3(0, -9.81f, 0)) : gravity(g) {}

    void addBody(PXIRigidBody* body);
    void step(float dt);
};
