#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace Physics
{
    struct AABB
    {
        glm::vec3 min;
        glm::vec3 max;

        AABB() : min(0.0f), max(0.0f) {}

        AABB(const glm::vec3& center, const glm::vec3& halfExtents) : min(center - halfExtents), max(center + halfExtents) {}

        static AABB FromTransform(const glm::vec3& position, const glm::vec3& scale)
        {
            glm::vec3 halfExtents = scale * 0.5f;
            return AABB(position, halfExtents);
        }

        glm::vec3 GetCenter() const {return (min + max) * 0.5f;}
        glm::vec3 GetExtents() const {return (max - min) * 0.5f;}
    };

    struct PhysicsComponent
    {
        glm::vec3 velocity{0.0f};
        glm::vec3 acceleration{0.0f};
        float mass = 1.0f;
        bool bUseGravity = true;
        bool bIsKinematic = false;

        bool bGrounded = false;
        glm::vec3 groundNormal{0.0f, 1.0f, 0.0f};
    };

    struct CollisionInfo
    {
        bool bCollided = false;
        int otherEntityID = -1;
        glm::vec3 contactAxis{0.0f};
        float contactDepth = 0.0f;
    };

    enum class CollisionType { NONE, COIN, ENEMY, PLATFORM, PLAYER, OBSTACLE, OTHER };

    using CollisionCallback = void(*)(int selfIndex, int otherIndex, CollisionType selfType, CollisionType otherType);
}
