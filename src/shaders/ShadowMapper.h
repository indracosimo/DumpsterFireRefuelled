// #pragma once
// #include <vector>
// #include <glm/glm.hpp>
// #include <glad/glad.h>
// #include "light.h"
// #include "shaders/shader.h"
//
// struct CubeTransform;
//
// class ShadowMapper
// {
// public:
//     ShadowMapper();
//     ~ShadowMapper();
//
//     void initializeShadowMap(light& lightSource);
//
//     void beginShadowPass(light& lightSource, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const std::vector<CubeTransform>& cubes);
//     void endShadowPass();
//
//     void bindShadowMap(light& lightSource, Shader* shader, int textureUnit = 0);
//
//     void cleanUp();
//     
//     Shader* shadowDepthShader;
// private:
//
//     GLint previousFramebuffer = 0;
//     glm::ivec4 previousViewport;
//
//     void createShadowFramebuffer(light& lightSource);
//     void createShadowDepthTexture(light& lightSource);
//     glm::mat4 calculateLightViewMatrix(const light& lightSource);
//     glm::mat4 calculateLightProjectionMatrix(const light& lightSource);
// };
