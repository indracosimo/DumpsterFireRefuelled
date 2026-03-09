#pragma once
#include <glm/glm.hpp>
#include "../shaders/shader.h"
#include "light.h"

class BasicShadowMapper
{
public:
    BasicShadowMapper();
    ~BasicShadowMapper();

    void Init(light& lightSource);
    void BeginDirectionalLight(light& lightSource);
    void EndDirectionalLight(unsigned int windowWidth, unsigned int windowHeight);

    void SetModelMatrix(const glm::mat4& modelMatrix);


private:
    Shader* depthShader = nullptr;
    glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);

    static glm::mat4 BiasMatrix();
};
