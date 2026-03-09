#pragma once
#include <glm/glm.hpp>
#include <string>
#include "../core/config.h"
#include "glad/glad.h"

enum class LightType { POINT = 0, DIRECTIONAL = 1, SPOT = 2};

struct light
{
    std::string name;

    LightType type = LightType::POINT;
    bool bEnabled = true;
    bool bCastShadows = true;

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

    glm::vec4 diffuse  = glm::vec4(1.0f);
    glm::vec4 specular = glm::vec4(1.0f);
    glm::vec3 attenuation = glm::vec3(1.0f, 0.0f, 0.0f);

    float spotCutoff = 0.5f;
    int   spotExponent = 32;

    GLuint shadowFrameBuffer = 0;
    GLuint shadowDepthTexture = 0;

    glm::ivec2 shadowMapResolution = glm::ivec2(2048, 2048);
    float shadowMapZoom = 20.0f;

    glm::vec3 shadowMapRenderPosition = glm::vec3(0.0f);
    glm::mat4 shadowMapMatrix = glm::mat4(1.0f);

    light() = default;
};
