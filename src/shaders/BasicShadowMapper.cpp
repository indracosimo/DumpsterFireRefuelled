#include "BasicShadowMapper.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



glm::mat4 BasicShadowMapper::BiasMatrix()
{
    glm::mat4 bias(1.0f);
    bias[0][0] = 0.5f; bias[1][1] = 0.5f; bias[2][2] = 0.5f;
    bias[3][0] = 0.5f; bias[3][1] = 0.5f; bias[3][2] = 0.5f;
    return bias;
}

BasicShadowMapper::BasicShadowMapper()
{
    depthShader = new Shader("src/shaders/shadow_Depth.vs", "src/shaders/shadow_Depth.fs");
}

BasicShadowMapper::~BasicShadowMapper()
{
    delete depthShader;
}

void BasicShadowMapper::Init(light& lightSource)
{
    if (lightSource.shadowFrameBuffer != 0 && lightSource.shadowDepthTexture != 0) return;

    glGenFramebuffers(1, &lightSource.shadowFrameBuffer);
    glGenTextures(1, &lightSource.shadowDepthTexture);
    glBindTexture(GL_TEXTURE_2D, lightSource.shadowDepthTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, lightSource.shadowMapResolution.x, lightSource.shadowMapResolution.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    const float border[4] = { 1.f, 1.f, 1.f, 1.f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glBindFramebuffer(GL_FRAMEBUFFER, lightSource.shadowFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, lightSource.shadowDepthTexture, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void BasicShadowMapper::SetModelMatrix(const glm::mat4& modelMatrix)
{
    if (!depthShader)
        return;

    depthShader->use();
    depthShader->setMat4("modelMatrix", modelMatrix);
}

void BasicShadowMapper::BeginDirectionalLight(light& lightSource)
{
    Init(lightSource);

    glViewport(0, 0, lightSource.shadowMapResolution.x, lightSource.shadowMapResolution.y);
    glBindFramebuffer(GL_FRAMEBUFFER, lightSource.shadowFrameBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_FRONT);

    const glm::vec3 dir = glm::normalize(lightSource.direction);
    const glm::vec3 up(0.f, 1.f, 0.f);

    const float z = lightSource.shadowMapZoom;
    const float nearPlane = 0.1f;
    const float farPlane = 200.0f;

    const glm::mat4 lightView = glm::lookAt(lightSource.shadowMapRenderPosition, lightSource.shadowMapRenderPosition + dir, up);
    
    const glm::mat4 lightProj = glm::ortho(-z, z, -z, z, nearPlane, farPlane);
    
    lightSpaceMatrix = lightProj * lightView;

    lightSource.shadowMapMatrix = BiasMatrix() * lightSpaceMatrix;

    depthShader->use();
    depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
}

void BasicShadowMapper::EndDirectionalLight(unsigned int windowWidth, unsigned int windowHeight)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, (int)windowWidth, (int)windowHeight);

    // glCullFace(GL_BACK);
}



