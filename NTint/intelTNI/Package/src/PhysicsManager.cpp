#include "../include/PhysicsManager.h"

namespace DirectMod {

    // Axis-Aligned Bounding Box collision detection
    bool PhysicsManager::CheckCollision(const AABB& a, const AABB& b) {
        return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
               (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
               (a.min.z <= b.max.z && a.max.z >= b.min.z);
    }

    // Update physics state
    void PhysicsManager::Update(float deltaTime) {
        // Loop through all physics objects
        for (auto& object : physicsObjects) {
            // Apply gravity (if enabled)
            if (object.useGravity) {
                object.velocity.y += gravity * deltaTime;
            }

            // Update position using velocity
            object.position += object.velocity * deltaTime;

            // Generate updated AABB
            object.UpdateAABB();

            // Check for collisions with other objects
            for (auto& other : physicsObjects) {
                if (&object == &other) continue;

                if (CheckCollision(object.aabb, other.aabb)) {
                    // Handle collision response
                    ResolveCollision(object, other);
                }
            }
        }
    }
}
