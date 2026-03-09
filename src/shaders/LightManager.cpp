#include "LightManager.h"

LightManager* LightManager::instance = nullptr;

LightManager& LightManager::GetInstance()
{
    if (!instance) {instance = new LightManager();}
    return *instance;
}

LightManager::LightManager()
{
    light defaultLight;
    defaultLight.name = "Default Directional Light";
    defaultLight.type = LightType::DIRECTIONAL;
    defaultLight.bEnabled = true;
    defaultLight.bCastShadows = true;
    defaultLight.direction = glm::normalize(glm::vec3(-0.5f, -1.0f, -0.3f));
    defaultLight.position = glm::vec3(0.0f);
    defaultLight.shadowMapResolution = { 2048, 2048 };
    defaultLight.shadowMapZoom = 20.0f;
    defaultLight.shadowMapRenderPosition = glm::vec3(0.0f, 20.0f, 0.0f);
    Lights.push_back(defaultLight);
}

light* LightManager::AddLight(light& Light)
{
    Lights.push_back(Light);
    return &Lights.back();
}

void LightManager::RemoveLight(int index)
{
    if (index >= 0 && index < Lights.size()) {Lights.erase(Lights.begin() + index);}
}

light* LightManager::GetLight(int index)
{
    if (index >= 0 && index < Lights.size()) {return &Lights[index];}
    return nullptr;
}

int LightManager::GetLightCount() const
{
    return Lights.size();
}

const std::vector<light>& LightManager::GetAllLights() const
{
    return Lights;
}


