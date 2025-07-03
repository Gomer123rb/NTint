#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

namespace DirectMod {
    struct Vector3 {
        float x, y, z;
    };

    struct AABB {
        Vector3 min;
        Vector3 max;
    };

    class PhysicsManager {
    public:
        // Check for collision between two axis-aligned bounding boxes.
        static bool CheckCollision(const AABB& a, const AABB& b);
        // Stub: Update physics simulation.
        static void Update(float deltaTime);
    };
}

#endif // PHYSICSMANAGER_H
