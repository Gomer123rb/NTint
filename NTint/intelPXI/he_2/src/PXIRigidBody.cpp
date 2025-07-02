#include "PXIRigidBody.h"

void PXIRigidBody::applyForce(const PXIVec3& force) {
    PXIVec3 acceleration = force * (1.0f / mass);
    velocity = velocity + acceleration;
}

void PXIRigidBody::update(float dt) {
    position = position + velocity * dt;
}
