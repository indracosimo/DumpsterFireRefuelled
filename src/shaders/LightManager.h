#pragma once
#include "light.h"
#include <vector>
#include <memory>

class LightManager
{
public:
    static LightManager& GetInstance();

    light* AddLight(light& Light);
    void RemoveLight(int index);
    light* GetLight(int index);
    int GetLightCount() const;
    const std::vector<light>& GetAllLights() const;

    glm::vec4 globalAmbient = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);

private:
    static LightManager* instance;
    std::vector<light> Lights;

    LightManager();
    
    
};
