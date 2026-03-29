#include "../include/DirectMod.h"

namespace DirectMod {

    // DEFINE static member (fixes LNK2001)
    std::vector<PhysicsObject> PhysicsManager::physicsObjects;

    // DEFINE private collision resolver (fixes LNK2019)
    void PhysicsManager::ResolveCollision(PhysicsObject& a, PhysicsObject& b)
    {
        // Simple elastic collision response placeholder
        // Swap velocities
        Vector3 temp = a.velocity;
        a.velocity = b.velocity;
        b.velocity = temp;

        // Push objects apart slightly to avoid overlap
        a.position.x -= 0.01f;
        b.position.x += 0.01f;

        // Update bounding boxes
        a.UpdateAABB();
        b.UpdateAABB();
    }

    // Axis-Aligned Bounding Box collision detection
    bool PhysicsManager::CheckCollision(const AABB& a, const AABB& b) {
        return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
            (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
            (a.min.z <= b.max.z && a.max.z >= b.min.z);
    }

    // Update physics state
    void PhysicsManager::Update(float deltaTime) {
        for (auto& object : physicsObjects) {

            // Apply gravity
            if (object.useGravity) {
                object.velocity.y += gravity * deltaTime;
            }

            // Update position
            object.position += object.velocity * deltaTime;

            // Update bounding box
            object.UpdateAABB();

            // Collision checks
            for (auto& other : physicsObjects) {
                if (&object == &other) continue;

                if (CheckCollision(object.aabb, other.aabb)) {
                    ResolveCollision(object, other);
                }
            }
        }
    }

}
