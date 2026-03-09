#pragma once
#include <string>
#include <vector>
#include <glm/vec3.hpp>

namespace dataFrame::serialize
{
    struct TransformData
    {
        glm::vec3 position{0.0f};
        glm::vec3 rotRad{0.0f};
        glm::vec3 scale{1.0f};
    };

    struct CubeEntityData
    {
        std::string type = "Cube";
        std::string name;
        
        TransformData transform;

        std::string meshPath;
        std::string texturePath;
        std::string specularMapPath;

        std::string collisionType = "OBSTACLE";

        bool bUseGravity = true;
        bool bIsKinematic = false;
        float mass = 1.0f;
    };

    struct CameraData
    {
        glm::vec3 position{0.0f, 0.0f, 2.0f};
        glm::vec3 orientation{0.0f, 0.0f, -1.0f};
        glm::vec3 up{0.0f, 1.0f, 0.0f};    
    };

    struct LevelData
    {
        int version = 1;
        std::string levelName;
        std::vector<CubeEntityData> entities;
        CameraData camera;
    };
}