#include <iostream>
#include "PXIPhysicsWorld.h"

int main() {
    PXIPhysicsWorld world;

    PXIRigidBody box(PXIVec3(0, 10, 0), 1.0f);
    world.addBody(&box);

    float deltaTime = 0.016f;

    for (int i = 0; i < 60; ++i) {
        world.step(deltaTime);
        std::cout << "Time: " << i * deltaTime << " | Y pos: " << box.position.y << std::endl;
    }

    return 0;
}
