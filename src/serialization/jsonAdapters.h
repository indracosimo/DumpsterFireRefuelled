#pragma once
#include <nlohmann/json.hpp>
#include <glm/vec3.hpp>
#include "LevelTypes.h"
#include "../graphics/renderer.h"

namespace nlohmann
{
    template<>
    struct adl_serializer<glm::vec3>
    {
        static void to_json(json& j, const glm::vec3& vec)
        {
            j = json::array({ vec.x, vec.y, vec.z });
        }

        static void from_json(const json& j, glm::vec3& vec)
        {
            vec.x = j.at(0).get<float>();
            vec.y = j.at(1).get<float>();
            vec.z = j.at(2).get<float>();
        }
    };
}

namespace dataFrame::serialize
{
    using nlohmann::json;

    // void to_json(json& j, const glm::vec3& vec);
    // void from_json(const json& j, glm::vec3& vec);

    void to_json(json& j, const TransformData& transform);
    void from_json(const json& j, TransformData& transform);

    void to_json(json& j, const CubeEntityData& entity);
    void from_json(const json& j, CubeEntityData& entity);

    void to_json(json& j, const CubeTransform& cam);
    void from_json(const json& j, CubeTransform& cam);

    void to_json(json& j, const LevelData& lvl);
    void from_json(const json& j, LevelData& lvl);
}
