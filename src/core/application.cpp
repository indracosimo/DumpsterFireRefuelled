#define GLM_ENABLE_EXPERIMENTAL
#include <glad/glad.h>
#include "application.h"
#include <filesystem>
#include "../core/camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "../input/input.h"
#include "../graphics/objectLoader.h"
#include "../ui/uiManager.h"
#include <iostream>
#include <vector>
#include "EditorState.h"
#include "../MessageManager.h"
#include "../MessageQueue.h"
#include "MemoryChecker.h"
#include "MeshManager.h"
#include "../graphics/textureLoader.h"
#include "../serialization/LevelBridge.h"
#include "../serialization/level_IO.h"
#include "../physics/physicsAABB.h"
#include "../physics/physicsManager.h"
#include "../game/PlayerControllerBehavior.h"
#include "../game/GameBehaviorContext.h"
#include "../game/BehaviorSystem.h"
#include "../game/StageManager.h"

extern GLFWwindow* window;
EditorState editorState;
static bool g_shouldAdvanceStage = false;
static int g_playerIndex = -1;
static int g_collectedCoinIndex = -1;

application::application(const Resolution& res, const char* title)
    : SCR_WIDTH(res.width), SCR_HEIGHT(res.height), window(nullptr)
{
    initGLFW();
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, title, nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    initGLAD();
    setupCallbacks();
}

application::~application()
{
	glfwTerminate();
}

void application::initGLFW()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void application::initGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
    std::cout << "GL_VENDOR:   " << (const char*)glGetString(GL_VENDOR) << '\n'
        << "GL_RENDERER: " << (const char*)glGetString(GL_RENDERER) << '\n'
        << "GL_VERSION:  " << (const char*)glGetString(GL_VERSION) << std::endl;
}

void framebuffer_size_callback(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
}

void application::setupCallbacks()
{
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void OnCollision(int selfIndex, int otherIndex, Physics::CollisionType selfType, Physics::CollisionType otherType)
{
    if (selfType == Physics::CollisionType::PLAYER && otherType == Physics::CollisionType::COIN)
    {
        g_shouldAdvanceStage = true;
        g_playerIndex = selfIndex;
        g_collectedCoinIndex = otherIndex;
    }
    else if (selfType == Physics::CollisionType::COIN && otherType == Physics::CollisionType::PLAYER)
    {
        g_shouldAdvanceStage = true;
        g_playerIndex = otherIndex;
        g_collectedCoinIndex = selfIndex;
    }
}


void application::Run()
{
    MeshManager::Allocate();
    camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0, 0, 2));
    renderer renderer(SCR_WIDTH, SCR_HEIGHT);
    uiManager uiManager;
	float FOV = 45.0f;

    
    uiManager.Initialize(window);

    std::vector<CubeTransform> cubes;
    CubeTransform newCube;
    int selectedCubeIndex = -1;
    bool mipmapEnabled = true;
    float lastFrame = (float)glfwGetTime();
    

    Physics::PhysicsManager physicsManager;
    physicsManager.Initialize(cubes.size());
    
    physicsManager.SetCollisionCallback(OnCollision);

    BehaviorSystem behaviorSystem;
    GameBehaviorContext gameBehaviorContext;
    gameBehaviorContext.window = this->window;
    gameBehaviorContext.cubes = &cubes;
    gameBehaviorContext.physics = &physicsManager;

    std::string levelPath = "assets/levels/CoolSpot.json";
    std::string levelName = "CoolSpot";

    StageManager stageManager;

    stageManager.AddStage(
        glm::vec3(0.0f, 2.0f, 0.0f),      // player spawn
        glm::vec3(0.0f, 5.0f, 15.0f),     // camera position
        glm::vec3(0.0f, 0.0f, -1.0f)      // camera orientation
    );

    stageManager.AddStage(
        glm::vec3(50.0f, 2.0f, 0.0f),
        glm::vec3(50.0f, 5.0f, 15.0f),
        glm::vec3(0.0f, 0.0f, -1.0f)
    );

    stageManager.AddStage
    (
        glm::vec3(100.0f, 2.0f, 0.0f),
        glm::vec3(100.0f, 5.0f, 15.0f),
        glm::vec3(0.0f, 0.0f, -1.0f)
    );
    
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        if (deltaTime <= 0.0f) deltaTime = 1.0f / 60.0f;
        lastFrame = currentFrame;
        


        
        processInput(window);
        const bool allowCamTranslation = (editorState.currentState != EditorState::State::PLAY);
        camera.Inputs(window, deltaTime, allowCamTranslation);
        
        // if (editorState.currentState == EditorState::State::PLAY && !editorState.bPaused)
        // {
        //     physicsManager.Update(deltaTime * editorState.playModeSpeed);
        //     physicsManager.ApplyPhysicsToTransforms(cubes);
        // }


    #pragma region Memory Status Check
        //check memory status
        MemoryChecker status = queryMemoryStatus();
        const uint64_t safetyMarginBytes = 1024 * 1024 * 1024; // 1 GB
        //Physical Memory Check
        if (status.availablePhysicalMemoryBytes < safetyMarginBytes)
        {
            std::cerr << "Warning: Low physical memory available!" << std::endl;
            std::cerr << "Total Physical Memory: " << status.totalPhysicalMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
            std::cerr << "Available Physical Memory: " << status.availablePhysicalMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
        }
        else
        {
            std::cout << "Memory Status: " << std::endl;
            std::cout << "Total Physical Memory: " << status.totalPhysicalMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
            std::cout << "Available Physical Memory: " << status.availablePhysicalMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
        }
        //Virtual Memory Check
        if (status.availableVirtualMemoryBytes < safetyMarginBytes)
        {
            std::cerr << "Warning: Low virtual memory available!" << std::endl;
            std::cerr << "Total Virtual Memory: " << status.totalVirtualMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
            std::cerr << "Available Virtual Memory: " << status.availableVirtualMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
        }
        else
        {
            std::cout << "Memory Status: " << std::endl;
            std::cout << "Total Virtual Memory: " << status.totalVirtualMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
            std::cout << "Available Virtual Memory: " << status.availableVirtualMemoryBytes / (1024 * 1024 * 1024) << " GB" << std::endl;
        }
#pragma endregion



        uiManager.BeginFrame();
        LightManager& lightMGR = LightManager::GetInstance();
        uiManager.RenderLightingControls(lightMGR);

        uiManager.RenderTextureOptions(mipmapEnabled, cubes);
	//process messages
		g_messageQueue.processAllMessages();

        //uiManager.RenderDocking();

        {
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);

#pragma region Runtime
    if (ImGui::Begin("Runtime/Editor"))
    { // Play/Edit toggle

        if (ImGui::Button(editorState.currentState == EditorState::State::PLAY ? "Edit##mode" : "Play##mode", ImVec2(100, 0)))
        {
            if (editorState.currentState == EditorState::State::EDIT)
            {
                // switch to PLAY 
                editorState.currentState = EditorState::State::PLAY;
                editorState.bPaused = false;
                stageManager.Reset();
                g_shouldAdvanceStage = false;
                std::cout << "PLAY window=" << (void*)gameBehaviorContext.window
          << " focused=" << glfwGetWindowAttrib(gameBehaviorContext.window, GLFW_FOCUSED)
          << "\n";
        
                // store original transforms
                editorState.originalPositions.clear();
                editorState.originalRotations.clear();
                editorState.originalScales.clear();
                for (const auto& cube : cubes)
                {
                    editorState.originalPositions.push_back(cube.position);
                    editorState.originalRotations.push_back(cube.rotation);
                    editorState.originalScales.push_back(cube.scale);
                }
        
                // init physics first
                physicsManager.Initialize(cubes.size());
                for (size_t i = 0; i < cubes.size(); ++i)
                    physicsManager.AddPhysicsComponent((int)i, cubes[i].physics, cubes[i].collisionType);
        
                // build behaviors
                behaviorSystem.ExitPlay(gameBehaviorContext);
                behaviorSystem.Clear(gameBehaviorContext);

                std::cout << "Cubes at EnterPlay:\n";
                for (size_t i = 0; i < cubes.size(); ++i)
                {
                    std::cout << "  i=" << i
                              << " name=" << cubes[i].name
                              << " collisionType=" << (int)cubes[i].collisionType
                              << "\n";
                }
                
                size_t playerIndex = (size_t)-1;
                for (size_t i = 0; i < cubes.size(); ++i)
                {
                    if (cubes[i].collisionType == Physics::CollisionType::PLAYER)
                    {
                        playerIndex = i;
                        break;
                    }
                }

                std::cout << "playerIndex = " << playerIndex
          << " cubes.size()=" << cubes.size()
          << " state=PLAY" << "\n";
                
                if (playerIndex != (size_t)-1)
                    behaviorSystem.Add<PlayerControllerBehavior>(playerIndex);
                std::cout << "playerIndex = " << playerIndex << "\n";
                behaviorSystem.EnterPlay(gameBehaviorContext);
            }
            else
            {
                // switch to EDIT 
                behaviorSystem.ExitPlay(gameBehaviorContext);
                behaviorSystem.Clear(gameBehaviorContext);
        
                editorState.currentState = EditorState::State::EDIT;
                editorState.bPaused = false;
        
                // restore transforms
                for (size_t i = 0; i < cubes.size() && i < editorState.originalPositions.size(); ++i)
                {
                    cubes[i].position = editorState.originalPositions[i];
                    cubes[i].rotation = editorState.originalRotations[i];
                    cubes[i].scale    = editorState.originalScales[i];
        
                    Physics::PhysicsComponent* physComp = physicsManager.GetPhysicsComponent((int)i);
                    if (physComp)
                    {
                        physComp->velocity = glm::vec3(0.0f);
                        physComp->acceleration = glm::vec3(0.0f);
                    }
                }
            }
        }
        ImGui::SameLine();
        if (editorState.currentState == EditorState::State::PLAY)
        {
            if (ImGui::Button(editorState.bPaused ? "Resume##pause" : "Pause##pause"))
            {
                editorState.bPaused = !editorState.bPaused;
            }
        }
        if (editorState.currentState == EditorState::State::PLAY)
        {
            ImGui::SliderFloat("Play Speed", &editorState.playModeSpeed, 0.0f, 2.0f);
        }

        ImGui::Separator();
        ImGui::Text("Global Physics Settings:");

        static float globalGravity = -9.81f;
        if(ImGui::DragFloat("Gravity", &globalGravity, 0.01f, -50.0f, 10.0f))
        {
            physicsManager.SetGravity(globalGravity);
        }
        
        ImGui::End();
    }
}

//  update physics
if (editorState.currentState == EditorState::State::PLAY && !editorState.bPaused)
{
    float simDeltaTime = deltaTime * editorState.playModeSpeed;
    
    behaviorSystem.Update(gameBehaviorContext, simDeltaTime);
    physicsManager.Update(simDeltaTime, cubes);
    physicsManager.ApplyPhysicsToTransforms(cubes, simDeltaTime);
    
    // physicsManager.Update(deltaTime * editorState.playModeSpeed);
    // physicsManager.ApplyPhysicsToTransforms(cubes);
}

        uiManager.RenderCubeControls(cubes, selectedCubeIndex, newCube, physicsManager);

        
#pragma endregion
      
#pragma region Level Editor
        ImGui::Begin("Level");
        {
            static char pathBuf[256];
            strncpy_s(pathBuf, levelPath.c_str(), sizeof(pathBuf));
            if (ImGui::InputText("Path", pathBuf, sizeof(pathBuf)))
                levelPath = pathBuf;

            static char nameBuf[128];
            strncpy_s(nameBuf, levelName.c_str(), sizeof(nameBuf));
            if (ImGui::InputText("Level Name", nameBuf, sizeof(nameBuf)))
                levelName = nameBuf;

            if (ImGui::Button("Save Level"))
            {
                dataFrame::serialize::LevelData lvl = dataFrame::serialize::ExportLevelFromCubes(cubes, levelName, camera);
                dataFrame::serialize::SaveLevelToFile(lvl, levelPath);
            }

            ImGui::SameLine();

            if (ImGui::Button("Load Level"))
            {
                dataFrame::serialize::LevelData lvl;
                if (dataFrame::serialize::LoadLevelFromFile(lvl, levelPath))
                {
                    dataFrame::serialize::ImportLevelToCubes(lvl, cubes, camera);
                    selectedCubeIndex = cubes.empty() ? -1 : 0;

                    physicsManager.Initialize(cubes.size());
                    
                    for (size_t i = 0; i < cubes.size(); ++i)
                    {
                        Physics::PhysicsComponent loadedPhys;
                        loadedPhys.bUseGravity = cubes[i].physics.bUseGravity;
                        loadedPhys.bIsKinematic = cubes[i].physics.bIsKinematic;
                        loadedPhys.mass = cubes[i].physics.mass;
                        loadedPhys.velocity = glm::vec3(0.0f);
                        loadedPhys.acceleration = glm::vec3(0.0f);
            
                        physicsManager.AddPhysicsComponent
                        (
                            (int)i,
                            loadedPhys,
                            cubes[i].collisionType
                        );
                    }

                    for (auto& c : cubes)
                    {
                        c.textureID = 0;
                        c.specularMapID = 0;

                        if (!c.texturePath.empty())
                            c.textureID = loadTexture(c.texturePath.c_str(), mipmapEnabled);

                        if (!c.specularMapPath.empty())
                            c.specularMapID = loadTexture(c.specularMapPath.c_str(), mipmapEnabled);
                    }
                    levelName = lvl.levelName;
                }
            }
#pragma region Levels
            
            if (g_shouldAdvanceStage && editorState.currentState == EditorState::State::PLAY)
            {
                g_shouldAdvanceStage = false;

                if (g_collectedCoinIndex >= 0 && g_collectedCoinIndex < (int)cubes.size())
                {
                    cubes[g_collectedCoinIndex].position = glm::vec3(-9999.0f, -9999.0f, -9999.0f);
                    physicsManager.SetEntityType(g_collectedCoinIndex, Physics::CollisionType::NONE);
                    g_collectedCoinIndex = -1;
                }
                
                if (stageManager.HasNextStage())
                {
                    stageManager.NextStage();
                    const StageData& stage = stageManager.GetCurrentStage();
        
                    if (g_playerIndex >= 0 && g_playerIndex < (int)cubes.size())
                    {
                        cubes[g_playerIndex].position = stage.playerSpawn;
            
                        Physics::PhysicsComponent* playerPhys = physicsManager.GetPhysicsComponent(g_playerIndex);
                        if (playerPhys)
                        {
                            playerPhys->velocity = glm::vec3(0.0f);
                            playerPhys->acceleration = glm::vec3(0.0f);
                        }
                    }
        
                    // tele camera
                    camera.Position = stage.cameraPosition;
                    camera.Orientation = stage.cameraOrientation;
        
                    std::cout << "Next stage" << stageManager.currentStage + 1 << std::endl;
                }
                else
                {
                    std::cout << "Woah you beat this piece of shit?!" << std::endl;
                    
                    editorState.currentState = EditorState::State::EDIT;
                }
            }
#pragma endregion
            if (ImGui::Button("New Level File"))
            {
                // auto-generate a new path
                static int newFileCounter = 0;
                std::stringstream ss;
                ss << "assets/levels/" << levelName << "_new_" << newFileCounter++ << ".json";
                levelPath = ss.str();
            }

            ImGui::SameLine();
            
            if (ImGui::Button("Save Level"))
            {
                std::string autoPath = dataFrame::serialize::GenerateLevelFilename(levelName);
                dataFrame::serialize::LevelData lvl = dataFrame::serialize::ExportLevelFromCubes(cubes, levelName, camera);
                bool success = dataFrame::serialize::SaveLevelToFile(lvl, autoPath);
                
                std::cout << "Attempting to save to: " << std::filesystem::absolute(autoPath) << std::endl;
                std::cout << "Save successful: " << (success ? "YES" : "NO") << std::endl;
                std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;
    
                levelPath = autoPath;
            }
        }
        ImGui::End();

#pragma endregion
		uiManager.RenderCameraControls(camera, FOV);
        {
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always);
            ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            ImGui::Begin("FPS", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
            ImGui::Text("FPS: %.1f", 1.0f / deltaTime);
            ImGui::Text("Frame: %.2f ms", deltaTime * 1000.0f);
            ImGui::End();
        }

        renderer.mainShader->use();
        camera.Matrix(FOV, 0.1f, 100.0f, *renderer.mainShader, "modelMatrix", "viewMatrix", "projectionMatrix");

        //planeMesh.DrawMesh();

        renderer.render(cubes, camera);
        uiManager.EndFrame();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    uiManager.Shutdown();
}