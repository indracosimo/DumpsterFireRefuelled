#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "uiManager.h"
#include <GLFW/glfw3.h>
#include <string>
#include "MessageQueue.h"
#include "../graphics/textureLoader.h"
#include "../graphics/renderer.h"
#include <core/camera.h>

#include "../shaders/LightManager.h"

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>

extern GLFWwindow* window;
 
uiManager::uiManager() 
{
	return;
}

uiManager::~uiManager() 
{
	if (bInitialized) 
	{
		Shutdown(); 
	}
}

void uiManager::Initialize(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 1.0f;
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bInitialized = true;
}

void uiManager::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    bInitialized = false;
}

void uiManager::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void uiManager::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

#pragma region Cube Controls
void uiManager::RenderCubeControls(std::vector<CubeTransform>& cubes, int& selectedCubeIndex, CubeTransform& newCubeTransform, Physics::PhysicsManager& physicsManager)
{
    ImGui::Begin("Cube Controls");
    ImGui::Text("Total Cubes: %d", static_cast<int>(cubes.size()));
    ImGui::Separator();

    ImGui::Text("New Cube Transform:");
    ImGui::Text("          x                y                z     ");
    ImGui::DragFloat3("Position##new", &newCubeTransform.position.x, 0.1f, -10.0f, 10.0f);

    glm::vec3 newRotDeg = glm::degrees(newCubeTransform.rotation);
    if (ImGui::DragFloat3("Rotation (deg)##new", &newRotDeg.x, 1.0f, 0.0f, 360.0f))
    {
        newCubeTransform.rotation = glm::radians(newRotDeg);
    }
    // ImGui::DragFloat3("Rotation##new", &newCubeTransform.rotation.x, 1.0f, 0.0f, 360.0f);
    ImGui::DragFloat3("Scale##new", &newCubeTransform.scale.x, 0.1f, 0.1f, 10.0f);

    ImGui::Separator();
    ImGui::Text("Physics Settings:");
            
    constexpr const char* CollisionTypeNames[] =
    { 
        "NONE",
        "COIN",
        "ENEMY",
        "PLATFORM",
        "PLAYER",
        "OBSTACLE",
        "OTHER"
    };
    constexpr int CollisionTypeCount = 7; 
            
    int newTypeIndex = static_cast<int>(newCubeTransform.collisionType);
    if (ImGui::Combo("Collision Type##new", &newTypeIndex, CollisionTypeNames, CollisionTypeCount))
    {
        newCubeTransform.collisionType = static_cast<Physics::CollisionType>(newTypeIndex);
    }
    
    if (ImGui::Button("Spawn Cube"))
    {
        if (newCubeTransform.name.empty() || newCubeTransform.name == "Nameless Cube" || newCubeTransform.name.rfind("Nameless Cube", 0) == 0)
        {
            newCubeTransform.name = "Nameless Cube " + std::to_string(cubes.size());
        }

        Physics::PhysicsComponent newPhysics;
        newPhysics.bUseGravity = true;
        newPhysics.mass = 1.0f;

        newCubeTransform.physics = newPhysics;
        
        newCubeTransform.texturePath = "assets/images/concrete.jpg";
        newCubeTransform.textureID = loadTexture(newCubeTransform.texturePath.c_str());

        if (newCubeTransform.specularMapPath.empty())
            newCubeTransform.specularMapPath = "assets/images/bricks.png";
        newCubeTransform.specularMapID = loadTexture(newCubeTransform.specularMapPath.c_str());
        
        cubes.push_back(newCubeTransform);
        selectedCubeIndex = static_cast<int>(cubes.size()) - 1;
        
        physicsManager.AddPhysicsComponent
        (
        selectedCubeIndex,
        cubes[selectedCubeIndex].physics,
        cubes[selectedCubeIndex].collisionType
        );
        
       

        //push cubes into message queue
        g_messageQueue.push(std::make_unique<CubeSpawnedMessage>(newCubeTransform));

        newCubeTransform.name = "Nameless Cube";
    }

    if (ImGui::Button("Reset Cube##new"))
    {
        newCubeTransform.position = { 0.0f, 0.0f, 0.0f };
        newCubeTransform.rotation = { 0.0f, 0.0f, 0.0f };
        newCubeTransform.scale = { 1.0f, 1.0f, 1.0f };
        newCubeTransform.name = "Nameless Cube";
    }


    ImGui::Separator();

    if (!cubes.empty())
    {
        ImGui::Text("Edit Existing Cubes:");

        if (ImGui::BeginCombo("Select Cube", selectedCubeIndex >= 0 ?
            cubes[selectedCubeIndex].name.c_str() : "None"))
        {
            for (int i = 0; i < cubes.size(); i++)
            {
                bool isSelected = (selectedCubeIndex == i);
                if (ImGui::Selectable(cubes[i].name.c_str(), isSelected))
                    selectedCubeIndex = i;
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (selectedCubeIndex >= 0 && selectedCubeIndex < cubes.size())
        {
            CubeTransform& cube = cubes[selectedCubeIndex];
            //texture loading in runtime stuff
            //exposes cubes texture path in ui, path is updated by user and the texture is reloaded during runtime
            static char texturePathBuffer[256];
            strncpy_s(texturePathBuffer, cube.texturePath.c_str(), sizeof(texturePathBuffer));
            if (ImGui::InputText("Texture Path", texturePathBuffer, sizeof(texturePathBuffer)))
            {
                cube.texturePath = texturePathBuffer;
                cube.textureID = loadTexture(cube.texturePath.c_str());
            }
            //ui for specular map
            static char specularPathBuffer[256];
            strncpy_s(specularPathBuffer, cube.specularMapPath.c_str(), sizeof(specularPathBuffer));
            if (ImGui::InputText("Specular Map Path", specularPathBuffer, sizeof(specularPathBuffer)))
            {
                cube.specularMapPath = specularPathBuffer;
                cube.specularMapID = loadTexture(cube.specularMapPath.c_str());
            }
            
            std::vector<char> cubeNameBuffer(cube.name.begin(), cube.name.end());
            cubeNameBuffer.resize(256);

            ImGui::Text("Cube %d Transform:", selectedCubeIndex, cubeNameBuffer.data());
            ImGui::Text("       x                y                z        ");


            if (ImGui::InputText("Cube Name", cubeNameBuffer.data(), cubeNameBuffer.size()))
            {
                cube.name = std::string(cubeNameBuffer.data());
            }
            //load mesh via obj
            static char meshPathBuffer[256];
            strncpy_s(meshPathBuffer, cube.meshPath.c_str(), sizeof(meshPathBuffer));
            if (ImGui::InputText("Mesh Path (.obj)", meshPathBuffer, sizeof(meshPathBuffer)))
            {
                cube.meshPath = meshPathBuffer;
            }

            
            ImGui::DragFloat3("Position", &cube.position.x, 0.1f, -10.0f, 999.0f);
            // ImGui::DragFloat3("Rotation", &cube.rotation.x, 1.0f, 0.0f, 360.0f);
            glm::vec3 rotDeg = glm::degrees(cube.rotation);
            if (ImGui::DragFloat3("Rotation (deg)", &rotDeg.x, 1.0f, 0.0f, 360.0f))
            {
                cube.rotation = glm::radians(rotDeg);
            }
            ImGui::DragFloat3("Scale", &cube.scale.x, 0.1f, 0.1f, 999.0f);
            //enum settings for physics
            ImGui::Separator();
            ImGui::Text("Individual Physics Settings:");
            
            constexpr const char* CollisionTypeNames[] =
            { 
                "NONE",
                "COIN",
                "ENEMY",
                "PLATFORM",
                "PLAYER",
                "OBSTACLE",
                "OTHER"
            };
            constexpr int CollisionTypeCount = 7; 
            
            int typeIndex = static_cast<int>(cube.collisionType);
            if (ImGui::Combo("Collision Type", &typeIndex, CollisionTypeNames, CollisionTypeCount))
            {
                cube.collisionType = static_cast<Physics::CollisionType>(typeIndex);
            }

            ImGui::Separator();
            ImGui::Text("Physics Component:");
            
            if (ImGui::Checkbox("Use Gravity##cube", &cube.physics.bUseGravity))
            {
                // gravity toggle 
            }
            
            if (ImGui::Checkbox("Is Kinematic##cube", &cube.physics.bIsKinematic))
            {
                // kinematic toggle 
            }
            
            ImGui::DragFloat("Mass##cube", &cube.physics.mass, 0.1f, 0.1f, 100.0f);
            ImGui::DragFloat3("Velocity##cube", &cube.physics.velocity.x, 0.1f, -50.0f, 50.0f);
            ImGui::DragFloat3("Acceleration##cube", &cube.physics.acceleration.x, 0.1f, -50.0f, 50.0f);
            
            if (ImGui::Button("Reset Cube"))
            {
                cube.position = { 0.0f, 0.0f, 0.0f };
                cube.rotation = { 0.0f, 0.0f, 0.0f };
                cube.scale = { 1.0f, 1.0f, 1.0f };
            }

            if (ImGui::Button("Delete Cube"))
            {
                cubes.erase(cubes.begin() + selectedCubeIndex);
                selectedCubeIndex = -1;

                for (int j = 0; j < (int)cubes.size(); ++j)
                {
                    if (cubes[j].name.rfind("Nameless Cube", 0) == 0)
                        cubes[j].name = "Nameless Cube " + std::to_string(j);
                }
            }
        }
    }

    if (ImGui::Button("Delete All Cubes"))
    {
        cubes.clear();
        selectedCubeIndex = -1;
    }

    ImGui::End();
}
#pragma endregion

#pragma region Camera Controls
void uiManager::RenderCameraControls(camera& camera, float& FOV)
{
	ImGui::Begin("Camera Controls");

	ImGui::SliderFloat("FOV", &FOV, 30.0f, 120.0f, "%.1f degree");

    ImGui::Text("Position");
    ImGui::DragFloat3("Camera Position", &camera.Position.x, 0.1f, -50.0f, 50.0f);

    ImGui::Text("Orientation:");
    ImGui::DragFloat3("Camera Orientation", &camera.Orientation.x, 0.1f, -1.0f, 1.0f);

    ImGui::Text("Up Vector:");
    ImGui::DragFloat3("Up", &camera.Up.x, 0.1f, -1.0f, 1.0f);

    ImGui::End();
}
#pragma endregion

#pragma region Lighting Controls
void uiManager::RenderLightingControls(LightManager& lightMGR)
{
     ImGui::Begin("Lighting Controls");
    
    ImGui::Text("Global Ambient");
    ImGui::ColorEdit4("##ambient", glm::value_ptr(lightMGR.globalAmbient));
    
    ImGui::Separator();
    ImGui::Text("Lights: %d", lightMGR.GetLightCount());
    
    if (ImGui::Button("Add Point Light"))
    {
        light newLight;
        newLight.name = "Point Light " + std::to_string(lightMGR.GetLightCount());
        newLight.type = LightType::POINT;
        lightMGR.AddLight(newLight);
    }
    ImGui::SameLine();
    if (ImGui::Button("Add Directional Light"))
    {
        light newLight{};

        newLight.name = "Directional Light " + std::to_string(lightMGR.GetLightCount());
        newLight.type = LightType::DIRECTIONAL;
        
        newLight.bEnabled = true;
        newLight.bCastShadows = true;

        newLight.direction = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));
        newLight.position = glm::vec3(0.0f);
        
        newLight.shadowMapResolution = { 2048, 2048 };
        newLight.shadowMapZoom = 20.0f;
        newLight.shadowMapRenderPosition = glm::vec3(0.0f, 20.0f, 0.0f);
        
        newLight.shadowFrameBuffer = 0;
        newLight.shadowDepthTexture = 0;

        lightMGR.AddLight(newLight);
    }
    ImGui::SameLine();
    if (ImGui::Button("Add Spotlight"))
    {
        light newLight;
        newLight.name = "Spotlight " + std::to_string(lightMGR.GetLightCount());
        newLight.type = LightType::SPOT;
        lightMGR.AddLight(newLight);
    }
    
    ImGui::Separator();
    
    for (int i = 0; i < lightMGR.GetLightCount(); i++)
    {
        light* light = lightMGR.GetLight(i);
        
        if (ImGui::TreeNode(light->name.c_str()))
        {
            ImGui::Checkbox("Enabled##light", &light->bEnabled);
            
            ImGui::DragFloat3("Position##light", glm::value_ptr(light->position), 0.1f);
            ImGui::DragFloat3("Direction##light", glm::value_ptr(light->direction), 0.01f, -1.0f, 1.0f);
            
            ImGui::ColorEdit4("Diffuse##light", glm::value_ptr(light->diffuse));
            ImGui::ColorEdit4("Specular##light", glm::value_ptr(light->specular));
            
            ImGui::DragFloat3("Attenuation##light", glm::value_ptr(light->attenuation), 0.01f, 0.0f, 10.0f);
            
            if (light->type == LightType::SPOT)
            {
                ImGui::SliderFloat("Spot Cutoff##light", &light->spotCutoff, 0.0f, 1.0f);
                ImGui::SliderInt("Spot Exponent##light", &light->spotExponent, 1, 128);
            }
            
            if (ImGui::Button("Delete Light"))
            {
                lightMGR.RemoveLight(i);
            }
            
            ImGui::TreePop();
        }
    }
    
    ImGui::End();
}
#pragma endregion

#pragma region Texture Options
void uiManager::RenderTextureOptions(bool& mipmapEnabled, std::vector<CubeTransform>& cubes)
{
    ImGui::Begin("Texture Options");

    if (ImGui::Checkbox("Enable Mipmaps", &mipmapEnabled))
    {
        for (auto& cube : cubes)
        {
            glDeleteTextures(1, &cube.textureID);
            cube.textureID = loadTexture(cube.texturePath.c_str(), mipmapEnabled);

            if (cube.specularMapID != 0) glDeleteTextures(1, &cube.specularMapID);
            if (!cube.specularMapPath.empty()) cube.specularMapID = loadTexture(cube.specularMapPath.c_str(), mipmapEnabled);
        }
    }
    ImGui::End();
    
}
#pragma endregion