#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <vector>  // For managing multiple physics objects

namespace DirectMod {

    struct Vector3 {
        float x, y, z;

        Vector3() : x(0), y(0), z(0) {}
        Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

        // Add two vectors
        Vector3 operator+(const Vector3& other) const {
            return {x + other.x, y + other.y, z + other.z};
        }

        // Multiply by scalar
        Vector3 operator*(float scalar) const {
            return {x * scalar, y * scalar, z * scalar};
        }

        // Add-assign (REQUIRED for PhysicsManager)
        Vector3& operator+=(const Vector3& other) {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }
    };

    struct AABB {
        Vector3 min;
        Vector3 max;

        void SetFromPositionSize(const Vector3& position, const Vector3& size) {
            min = position;
            max = {position.x + size.x, position.y + size.y, position.z + size.z};
        }
    };

    struct PhysicsObject {
        Vector3 position;
        Vector3 velocity;
        AABB aabb;
        bool useGravity = true;

        void UpdateAABB() {
            Vector3 size = {aabb.max.x - aabb.min.x,
                            aabb.max.y - aabb.min.y,
                            aabb.max.z - aabb.min.z};
            aabb.SetFromPositionSize(position, size);
        }
    };

    class PhysicsManager {
    public:
        static bool CheckCollision(const AABB& a, const AABB& b);
        static void Update(float deltaTime);

    private:
        static std::vector<PhysicsObject> physicsObjects;
        static constexpr float gravity = -9.81f;

        static void ResolveCollision(PhysicsObject& a, PhysicsObject& b);
    };
}

#endif // PHYSICSMANAGER_H
