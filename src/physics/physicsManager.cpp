#include "physicsManager.h"
#include <algorithm>
#include <iostream>
#include "../graphics/renderer.h"

namespace Physics
{
    PhysicsManager::PhysicsManager()
    {
    }

    void PhysicsManager::Initialize(size_t cubeCount)
    {
        physicsComponents.clear();
        physicsComponents.resize(cubeCount);
        
        collisionBoxes.clear();
        collisionBoxes.resize(cubeCount);
        
        entityTypes.clear();
        entityTypes.resize(cubeCount, CollisionType::OBSTACLE);
    }

    void PhysicsManager::Update(float deltaTime, std::vector<CubeTransform>& cubes)
    {
        if (deltaTime <= 0.0f) return;

        for (auto& phys : physicsComponents)
        {
            phys.bGrounded = false;
            phys.groundNormal = glm::vec3(0.0f, 1.0f, 0.0f);
        }

        for (size_t i = 0; i < physicsComponents.size(); i++)
        {
            PhysicsComponent& phys = physicsComponents[i];
            if (phys.bIsKinematic) continue;

            phys.acceleration.y = phys.bUseGravity ? gravity : 0.0f;
            phys.velocity += phys.acceleration * deltaTime;
        }
        // collision detection + resolution + separation
        for (size_t i = 0; i < physicsComponents.size(); i++)
        {
            for (size_t j = i + 1; j < physicsComponents.size(); j++)
            {
                CollisionInfo info;
                if (!bCheckAABBCollision(collisionBoxes[i], collisionBoxes[j], info))
                    continue;
                
                if (collisionCallback)
                {
                    collisionCallback((int)i, (int)j, entityTypes[i], entityTypes[j]);
                }
                
                const float slop = 0.001f;
                float sep = info.contactDepth + slop;
                glm::vec3 correction = info.contactAxis * sep;

                PhysicsComponent& physA = physicsComponents[i];
                PhysicsComponent& physB = physicsComponents[j];

                bool AStatic = physA.bIsKinematic || entityTypes[i] == CollisionType::OBSTACLE || entityTypes[i] == CollisionType::PLATFORM;
                bool BStatic = physB.bIsKinematic || entityTypes[j] == CollisionType::OBSTACLE || entityTypes[j] == CollisionType::PLATFORM;

                if (!AStatic && BStatic)
                {
                    cubes[i].position += correction;
                }
                else if (AStatic && !BStatic)
                {
                    cubes[j].position -= correction;
                }
                else if (!AStatic && !BStatic)
                {
                    cubes[i].position += correction * 0.5f;
                    cubes[j].position -= correction * 0.5f;
                }
                
                collisionBoxes[i] = AABB::FromTransform(cubes[i].position, cubes[i].scale);
                collisionBoxes[j] = AABB::FromTransform(cubes[j].position, cubes[j].scale);
                
                ResolveCollision((int)i, (int)j, info);

                if (entityTypes[i] == CollisionType::PLAYER && info.contactAxis.y > 0.5f)
                {
                    physicsComponents[i].bGrounded = true;
                    physicsComponents[i].groundNormal = info.contactAxis;
                }

                if (entityTypes[j] == CollisionType::PLAYER && info.contactAxis.y < -0.5f)
                {
                    physicsComponents[j].bGrounded = true;
                    physicsComponents[j].groundNormal = -info.contactAxis;
                }
            }
        }
        
        //
        // for (size_t i = 0; i < physicsComponents.size(); i++)
        // {
        //     PhysicsComponent& physComponent = physicsComponents[i];
        //
        //     if (physComponent.bIsKinematic) continue;
        //
        //     if (physComponent.bUseGravity)
        //     {
        //         physComponent.acceleration.y = gravity;
        //     }
        //     else
        //     {
        //         physComponent.acceleration.y = 0.0f;
        //     }
        //
        //     physComponent.velocity += physComponent.acceleration * deltaTime;
        // }
        //
        // //aabb collision detection
        // for (size_t i = 0; i < physicsComponents.size(); i++)
        // {
        //     for (size_t j = i + 1; j < physicsComponents.size(); j++)
        //     {
        //         CollisionInfo info;
        //         if (bCheckAABBCollision(collisionBoxes[i], collisionBoxes[j], info))
        //         {
        //             info.bCollided = true;
        //             info.otherEntityID = j;
        //             
        //             if (collisionCallback)
        //             {
        //                 collisionCallback(static_cast<int>(i), static_cast<int>(j), 
        //                                 entityTypes[i], entityTypes[j]);
        //             }
        //             
        //             ResolveCollision(static_cast<int>(i), static_cast<int>(j), info);
        //         }
        //     }
        // }
    }

    void PhysicsManager::SetCollisionCallback(CollisionCallback callback)
    {
        collisionCallback = callback;
    }

    void PhysicsManager::AddPhysicsComponent(int entityIndex, const PhysicsComponent& component, CollisionType type)
    {
        if (entityIndex >= 0 && entityIndex < static_cast<int>(physicsComponents.size()))
        {
            physicsComponents[entityIndex] = component;
            entityTypes[entityIndex] = type;
        }
    }

    void PhysicsManager::SetEntityType(int entityIndex, CollisionType type)
    {
        if (entityIndex >= 0 && entityIndex < static_cast<int>(entityTypes.size()))
        {
            entityTypes[entityIndex] = type;
        }
    }

    void PhysicsManager::ApplyPhysicsToTransforms(std::vector<CubeTransform>& cubes, float deltaTime)
    {
        for (size_t i = 0; i < cubes.size() && i < physicsComponents.size(); i++)
        {
            PhysicsComponent& phys = physicsComponents[i];
            CubeTransform& cube = cubes[i];

            if (!phys.bIsKinematic)
                cube.position += phys.velocity * deltaTime;
            
            collisionBoxes[i] = AABB::FromTransform(cube.position, cube.scale);
        }
    }

    void PhysicsManager::RemoveVelocityAlongAxis(PhysicsComponent& component, const glm::vec3& axis)
    {
        float vAlong = glm::dot(component.velocity, axis);
        if ((vAlong > 0.0f && glm::dot(axis, axis) > 0.0f) || (vAlong < 0.0f && glm::dot(axis, axis) > 0.0f))
            component.velocity -= axis * vAlong;
    }

    PhysicsComponent* PhysicsManager::GetPhysicsComponent(int entityIndex)
    {
        if (entityIndex >= 0 && entityIndex < static_cast<int>(physicsComponents.size()))
        {
            return &physicsComponents[entityIndex];
        }
        return nullptr;
    }

    AABB PhysicsManager::GetWorldAABB(int entityIndex, const CubeTransform& cubeTransform) const
    {
        return AABB::FromTransform(cubeTransform.position, cubeTransform.scale);
    }

    bool PhysicsManager::bCheckAABBCollision(const AABB& a, const AABB& b, CollisionInfo& outInfo)
    {
        outInfo = CollisionInfo{};

        const float overlapX = std::min(a.max.x, b.max.x) - std::max(a.min.x, b.min.x);
        const float overlapY = std::min(a.max.y, b.max.y) - std::max(a.min.y, b.min.y);
        const float overlapZ = std::min(a.max.z, b.max.z) - std::max(a.min.z, b.min.z);

        const float eps = 0.0001f;
        if (overlapX <= eps || overlapY <= eps || overlapZ <= eps)
            return false;

        float minOverlap = overlapX;
        glm::vec3 axis(1.0f, 0.0f, 0.0f);

        if (overlapY < minOverlap)
        {
            minOverlap = overlapY;
            axis = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if (overlapZ < minOverlap)
        {
            minOverlap = overlapZ;
            axis = glm::vec3(0.0f, 0.0f, 1.0f);
        }

        const glm::vec3 aCenter = a.GetCenter();
        const glm::vec3 bCenter = b.GetCenter();
        glm::vec3 dir = aCenter - bCenter;

        if (axis.x != 0.0f) axis.x = (dir.x >= 0.0f) ? 1.0f : -1.0f;
        if (axis.y != 0.0f) axis.y = (dir.y >= 0.0f) ? 1.0f : -1.0f;
        if (axis.z != 0.0f) axis.z = (dir.z >= 0.0f) ? 1.0f : -1.0f;

        outInfo.bCollided = true;
        outInfo.contactAxis = axis;
        outInfo.contactDepth = minOverlap;
        return true;
    }

    void PhysicsManager::ResolveCollision(int ID0, int ID1, const CollisionInfo& info)
    {
        PhysicsComponent& phys1 = physicsComponents[ID0];
        PhysicsComponent& phys2 = physicsComponents[ID1];
        
        bool is1Kinematic = phys1.bIsKinematic || (entityTypes[ID0] == CollisionType::OBSTACLE || entityTypes[ID0] == CollisionType::PLATFORM);
        bool is2Kinematic = phys2.bIsKinematic || (entityTypes[ID1] == CollisionType::OBSTACLE || entityTypes[ID1] == CollisionType::PLATFORM);
        
        if (is1Kinematic && is2Kinematic) return;
    
        float separationAmount = info.contactDepth + 0.001f; 
    
        if (is1Kinematic && !is2Kinematic)
        {
            RemoveVelocityAlongAxis(phys2, -info.contactAxis);
        }
        else if (!is1Kinematic && is2Kinematic)
        {
            RemoveVelocityAlongAxis(phys1, info.contactAxis);
        }
        else
        {
            float totalMass = phys1.mass + phys2.mass;
            float mass1Ratio = phys2.mass / totalMass;
            float mass2Ratio = phys1.mass / totalMass;
        
            phys1.velocity -= info.contactAxis * (phys1.velocity.y * mass1Ratio);
            phys2.velocity += info.contactAxis * (phys1.velocity.y * mass2Ratio);
        }
        
    }

    std::vector<int> PhysicsManager::GetNearbyEntities(int entityIndex, float searchRadius)
    {
        return {};
    }
}
