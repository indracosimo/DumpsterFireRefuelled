#pragma once
#include <vector>

struct GLFWwindow;
struct CubeTransform;

namespace Physics
{
    class PhysicsManager;
}

struct GameBehaviorContext
{
    GLFWwindow* window = nullptr;
    std::vector<CubeTransform>* cubes = nullptr;
    Physics::PhysicsManager* physics = nullptr;
};