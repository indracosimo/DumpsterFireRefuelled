#pragma once
#include <string>
#include <unordered_map>
#include "graphics/renderer.h"

class MeshManager
{
public:
    static void Allocate();
    static MeshManager& GetInstance();
    static void Destroy();

    Mesh* LoadMesh(const std::string& filepath);
    Mesh* GetMesh(const std::string& filepath);
    
private:
    static MeshManager* instance;
    std::unordered_map<std::string, Mesh*> loadedMeshes;

    MeshManager() = default;
};
