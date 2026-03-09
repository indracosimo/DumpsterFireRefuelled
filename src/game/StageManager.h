#pragma once
#include <glm/vec3.hpp>
#include <vector>

struct StageData
{
    glm::vec3 playerSpawn{0.0f};
    glm::vec3 cameraPosition{0.0f, 5.0f, 10.0f};
    glm::vec3 cameraOrientation{0.0f, 0.0f, -1.0f};
};

class StageManager
{
public:
    int currentStage = 0;
    std::vector<StageData> stages;
    
    void AddStage(const glm::vec3& playerSpawn, const glm::vec3& camPos, const glm::vec3& camOrient = glm::vec3(0.0f, 0.0f, -1.0f))
    {
        StageData stage;
        stage.playerSpawn = playerSpawn;
        stage.cameraPosition = camPos;
        stage.cameraOrientation = camOrient;
        stages.push_back(stage);
    }
    
    bool HasNextStage() const
    {
        return currentStage + 1 < (int)stages.size();
    }
    
    void NextStage()
    {
        if (HasNextStage())
            currentStage++;
    }
    
    const StageData& GetCurrentStage() const
    {
        return stages[currentStage];
    }
    
    void Reset()
    {
        currentStage = 0;
    }
};