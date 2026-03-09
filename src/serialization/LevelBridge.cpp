#include "LevelBridge.h"

namespace dataFrame::serialize
{
    LevelData ExportLevelFromCubes(const std::vector<CubeTransform>& cubes, const std::string& levelName, const camera& cam)
    {
        LevelData lvl;
        lvl.version = 1;
        lvl.levelName = levelName;
        lvl.entities.reserve(cubes.size());

        lvl.camera.position = cam.Position;
        lvl.camera.orientation = cam.Orientation;
        lvl.camera.up = cam.Up;

        for (const auto& c : cubes)
        {
            CubeEntityData e;
            e.type = "Cube";
            e.name = c.name;

            e.transform.position = c.position;
            e.transform.rotRad = c.rotation;   // runtime radians
            e.transform.scale = c.scale;

            e.meshPath = c.meshPath;
            e.texturePath = c.texturePath;
            e.specularMapPath = c.specularMapPath;

            e.collisionType = CollisionTypeToString(c.collisionType);

            e.bUseGravity = c.physics.bUseGravity;
            e.bIsKinematic = c.physics.bIsKinematic;
            e.mass = c.physics.mass;

            lvl.entities.push_back(std::move(e));
        }

        return lvl;
    }

    void ImportLevelToCubes(const LevelData& level, std::vector<CubeTransform>& outCubes, camera& cam)
    {
        outCubes.clear();
        outCubes.reserve(level.entities.size());

        cam.Position = level.camera.position;
        cam.Orientation = level.camera.orientation;
        cam.Up = level.camera.up;

        for (const auto& e : level.entities)
        {
            if (e.type != "Cube")
                continue;

            CubeTransform c;
            c.name = e.name;

            c.position = e.transform.position;
            c.rotation = e.transform.rotRad;   // runtime radians
            c.scale = e.transform.scale;

            c.meshPath = e.meshPath;
            c.texturePath = e.texturePath;
            c.specularMapPath = e.specularMapPath;

            c.physics.bUseGravity = e.bUseGravity;
            c.physics.bIsKinematic = e.bIsKinematic;
            c.physics.mass = e.mass;
            c.collisionType = StringToCollisionType(e.collisionType);
            

            outCubes.push_back(std::move(c));
        }
    }

    std::string CollisionTypeToString(Physics::CollisionType type)
    {
        switch (type)
        {
        case Physics::CollisionType::PLAYER:   return "PLAYER";
        case Physics::CollisionType::PLATFORM: return "PLATFORM";
        case Physics::CollisionType::ENEMY:    return "ENEMY";
        case Physics::CollisionType::COIN:     return "COIN";
        case Physics::CollisionType::OBSTACLE: return "OBSTACLE";
        case Physics::CollisionType::OTHER:    return "OTHER";
        default:                               return "NONE";
        }
    }
    
    Physics::CollisionType StringToCollisionType(const std::string& str)
    {
        if (str == "PLAYER")   return Physics::CollisionType::PLAYER;
        if (str == "PLATFORM") return Physics::CollisionType::PLATFORM;
        if (str == "ENEMY")    return Physics::CollisionType::ENEMY;
        if (str == "COIN")     return Physics::CollisionType::COIN;
        if (str == "OBSTACLE") return Physics::CollisionType::OBSTACLE;
        if (str == "OTHER")    return Physics::CollisionType::OTHER;
        return Physics::CollisionType::NONE;
    }
    
}
