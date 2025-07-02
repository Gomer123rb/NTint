#include "PXIPhysicsWorld.h"

void PXIPhysicsWorld::addBody(PXIRigidBody* body) {
    bodies.push_back(body);
}

void PXIPhysicsWorld::step(float dt) {
    for (auto* body : bodies) {
        body->applyForce(gravity * body->mass);
        body->update(dt);
    }
}
