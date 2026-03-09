#pragma once
#include <string>
#include <vector>
#include "LevelTypes.h"
#include "../graphics/renderer.h"
#include "../physics/physicsAABB.h"
#include "../core/camera.h"

namespace dataFrame::serialize
{
    LevelData ExportLevelFromCubes(const std::vector<CubeTransform>& cubes, const std::string& levelName, const camera& cam);
    void ImportLevelToCubes(const LevelData& level, std::vector<CubeTransform>& outCubes, camera& cam);
    
    std::string CollisionTypeToString(Physics::CollisionType type);
    Physics::CollisionType StringToCollisionType(const std::string& str);
}