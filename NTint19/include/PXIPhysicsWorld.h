#pragma once
#include <vector>
#include "PXIVec3.h"      // direct, minimal include

class PXIRigidBody;       // forward declaration

class PXIPhysicsWorld {
public:
    PXIVec3 gravity;
    std::vector<PXIRigidBody*> bodies;

    PXIPhysicsWorld(PXIVec3 g = PXIVec3(0, -9.81f, 0)) : gravity(g) {}

    void addBody(PXIRigidBody* body);
    void step(float dt);
};
