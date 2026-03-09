#pragma once
#include "../src/graphics/renderer.h"
#include "../MessageManager.h"
#include <vector>
#include "../core/camera.h"
#include "../shaders/LightManager.h"
struct ImGuiIO;
struct GLFWwindow;

class uiManager
{
public:
    uiManager();
    ~uiManager();

    void Initialize(GLFWwindow* window);
    void Shutdown();

    void BeginFrame();
    void EndFrame();

    void RenderCubeControls(std::vector<CubeTransform>& cubes, int& selectedCubeIndex, CubeTransform& newCubeTransform, Physics::PhysicsManager& physicsManager);

	void RenderCameraControls(camera& camera, float& FOV);
    void RenderLightingControls(LightManager& lightMGR);
    void RenderTextureOptions(bool& mipmapEnabled, std::vector<CubeTransform>& cubes);

    bool bInitialized = false;
};
