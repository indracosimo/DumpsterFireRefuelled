#include "jsonAdapters.h"

#include "glm/vec2.hpp"

namespace dataFrame::serialize
{
    // void to_json(json& j, const glm::vec3& vec)
    // {
    //     j = json::array({vec.x, vec.y, vec.z});
    // }
    //
    // void from_json(const json& j, glm::vec3& vec)
    // {
    //     vec.x = j.at(0).get<float>();
    //     vec.y = j.at(1).get<float>();
    //     vec.z = j.at(2).get<float>();
    // }
    void to_json(json& j, const TransformData& transform)
    {
        j = json
        {
                {"pos", transform.position},
                {"rotRad", transform.rotRad},
                {"scale", transform.scale}
        };
    }

    void from_json(const json& j, TransformData& transform)
    {
        transform.position = j.value("pos", glm::vec3(0.0f));
        transform.rotRad   = j.value("rotRad", glm::vec3(0.0f));
        transform.scale    = j.value("scale", glm::vec3(1.0f));
    }

    // void to_json(json& j, const TransformData& transform)
    // {
    //     j = json
    //     {
    //             {"Position", transform.position},
    //             {"rotRad", transform.rotRad},
    //             {"Scale", transform.scale}
    //     };
    // }
    //
    // void from_json(const json& j, TransformData& transform)
    // {
    //     transform.position = j.value("Position", glm::vec3(0.0f));
    //     transform.rotRad = j.value("rotRad", glm::vec3(0.0f));
    //     transform.scale = j.value("Scale", glm::vec3(1.0f));
    // }

    void to_json(json& j, const CubeEntityData& entity)
    {
        j = json
        {
                {"type", entity.type},
                {"name", entity.name},
                {"transform", entity.transform},
                {"meshPath", entity.meshPath},
                {"texturePath", entity.texturePath},
                {"specularMapPath", entity.specularMapPath},
                {"collisionType", entity.collisionType},
                {"bUseGravity", entity.bUseGravity},
                {"bIsKinematic", entity.bIsKinematic},
                {"mass", entity.mass}
        };
    }

    void from_json(const json& j, CubeEntityData& ent)
    {
        ent.type = j.value("type", "Cube");
        ent.name = j.value("name", "");
        ent.transform = j.value("transform", TransformData{});
        ent.meshPath = j.value("meshPath", "");
        ent.texturePath = j.value("texturePath", "");
        ent.specularMapPath = j.value("specularMapPath", "");
        ent.collisionType = j.value("collisionType", "Obstacle");
        ent.bUseGravity = j.value("bUseGravity", true);
        ent.bIsKinematic = j.value("bIsKinematic", false);
        ent.mass = j.value("mass", 1.0f);
    }

    void to_json(json& j, const CameraData& cam)
    {
        j = json
        {
            {"position", cam.position},
            {"orientation", cam.orientation},
            {"up", cam.up}
        };
    }
    
    void from_json(const json& j, CameraData& cam)
    {
        cam.position = j.value("position", glm::vec3(0.0f, 0.0f, 2.0f));
        cam.orientation = j.value("orientation", glm::vec3(0.0f, 0.0f, -1.0f));
        cam.up = j.value("up", glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void to_json(json& j, const LevelData& lvl)
    {
        j = json
        {
                {"version", lvl.version},
                {"levelName", lvl.levelName},
                {"entities", lvl.entities},
                {"camera", lvl.camera}
        };
    }

    void from_json(const json& j, LevelData& lvl)
    {
        lvl.version = j.value("version", 1);
        lvl.levelName = j.value("levelName", "");
        lvl.entities = j.value("entities", std::vector<CubeEntityData>{});
        lvl.camera = j.value("camera", CameraData{});

    }
}
