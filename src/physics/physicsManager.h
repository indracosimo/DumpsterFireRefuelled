#pragma once
#include "physicsAABB.h"
#include <vector>
#include <functional>

struct CubeTransform;

namespace Physics
{
    class PhysicsManager
    {
    private:
        //static constexpr
        float gravity = -9.81f; //"The conventional value for standard gravity is 9.80665 m⋅s^−2" /wikipedia Gravity of Earth
        std::vector<PhysicsComponent> physicsComponents;
        std::vector<AABB> collisionBoxes;
        std::vector<CollisionType> entityTypes;
        CollisionCallback collisionCallback = nullptr;

    public:
        PhysicsManager();
        
        void Initialize(size_t cubeCount);

        void Update(float deltaTime, std::vector<CubeTransform>& cubes);

        void SetCollisionCallback(CollisionCallback callback);

        void SetGravity(float newGravity) {gravity = newGravity;}
        float GetGravity() const { return gravity; }

        void AddPhysicsComponent(int entityIndex, const PhysicsComponent& component, CollisionType type);

        void SetEntityType(int entityIndex, CollisionType type);

        void ApplyPhysicsToTransforms(std::vector<CubeTransform>& cubes, float deltaTime);

        void RemoveVelocityAlongAxis(PhysicsComponent& component, const glm::vec3& axis);
        
        PhysicsComponent* GetPhysicsComponent(int entityIndex);
        AABB GetWorldAABB(int entityIndex, const CubeTransform& cubeTransform) const;

    private:

        bool bCheckAABBCollision(const AABB& a, const AABB& b, CollisionInfo& outInfo);
        void ResolveCollision(int ID0, int ID1, const CollisionInfo& info);

        std::vector<int> GetNearbyEntities(int entityIndex, float searchRadius);
    };
}
