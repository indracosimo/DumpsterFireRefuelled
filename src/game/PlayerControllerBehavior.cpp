#define GLFW_INCLUDE_NONE

#include "PlayerControllerBehavior.h"
#include "GameBehaviorContext.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "../graphics/renderer.h"
#include "../physics/physicsManager.h"

#include <cmath>

PlayerControllerBehavior::PlayerControllerBehavior(size_t cubeIndex) : m_cubeIndex(cubeIndex)
{}

bool PlayerControllerBehavior::bIsGrounded(const GameBehaviorContext& context, float deltaTime)
{
    if (!context.physics) return false;
    Physics::PhysicsComponent* phys = context.physics->GetPhysicsComponent((int)m_cubeIndex);
    if (!phys) return false;
    
    if (phys->bGrounded)
    {
        bTimeSinceGrounded = 0.0f;
        return true;
    }
    
    bTimeSinceGrounded += deltaTime;
    return bTimeSinceGrounded <= coyoteTime;
}

void PlayerControllerBehavior::Update(GameBehaviorContext& context, float /*deltaTime*/)
{
    if (!context.window || !context.cubes || !context.physics) return;
    if (m_cubeIndex >= context.cubes->size()) return;
    
    //auto& cubes = *context.cubes;
    
    //ImGuiIO& io = ImGui::GetIO();
    if (ImGui::GetIO().WantTextInput) return;

    Physics::PhysicsComponent* physComp = context.physics->GetPhysicsComponent((int)m_cubeIndex);
    if (!physComp) return;

    //world relative movement
    float x = 0.0f;
    float z = 0.0f;
    
    if (glfwGetKey(context.window, GLFW_KEY_A) == GLFW_PRESS) x -= 1.0f;
    if (glfwGetKey(context.window, GLFW_KEY_D) == GLFW_PRESS) x += 1.0f;
    if (glfwGetKey(context.window, GLFW_KEY_W) == GLFW_PRESS) z -= 1.0f;
    if (glfwGetKey(context.window, GLFW_KEY_S) == GLFW_PRESS) z += 1.0f;

    //normalize diagonal inputs
    const float lenSq = x * x + z * z;
    if (lenSq > 0.0001f)
    {
        const float invLen = 1.0f / sqrtf(lenSq);
        x *= invLen;
        z *= invLen;
    }

    physComp->velocity.x = x * moveSpeed;
    physComp->velocity.z = z * moveSpeed;
    (*context.cubes)[m_cubeIndex].physics.velocity = physComp->velocity;
    
    bool jumpDown = (glfwGetKey(context.window, GLFW_KEY_SPACE) == GLFW_PRESS);
    bool jumpPressed = jumpDown && !bm_prevJumpDown;
    bm_prevJumpDown = jumpDown;

    if (jumpPressed && bIsGrounded(context, 999))
    {
        physComp->velocity.y = jumpSpeed;
        (*context.cubes)[m_cubeIndex].physics.velocity.y = jumpSpeed;
        bTimeSinceGrounded = 999.0f; 
    }
    
    // if (glfwGetKey(context.window, GLFW_KEY_W) == GLFW_PRESS)
    // {
    //         (*context.cubes)[m_cubeIndex].position.z -= moveSpeed * 0.016f;
    //     static float t = 0.0f;
    //     t += 0.016f;
    //     if (t > 1.0f) { printf("PlayerControllerBehavior running\n"); t = 0.0f; }
    // }
    // if (glfwGetKey(context.window, GLFW_KEY_S) == GLFW_PRESS)
    // {
    //     (*context.cubes)[m_cubeIndex].position.z += moveSpeed * 0.016f;
    //     static float t = 0.0f;
    //     t += 0.016f;
    //     if (t > 1.0f) { printf("PlayerControllerBehavior running\n"); t = 0.0f; }
    // }
    
    //uncomment if cubes sink below ground
    // if (cubes[m_cubeIndex].position.y < 0.0f)
    //     cubes[m_cubeIndex].position.y = 0.0f;
}