#include "MeshManager.h"

#include "graphics/objectLoader.h"

MeshManager* MeshManager::instance = nullptr;

void MeshManager::Allocate()
{
    if (!instance) instance = new MeshManager();
}

MeshManager& MeshManager::GetInstance()
{
    return *instance;
}

void MeshManager::Destroy()
{
    for (auto& mesh : instance->loadedMeshes) delete mesh.second;
    delete instance;
}

Mesh* MeshManager::LoadMesh(const std::string& filepath)
{
    if (loadedMeshes.contains(filepath)) return loadedMeshes[filepath];

    Mesh newMesh = objectLoader::loadOBJ(filepath);
    Mesh* storedMesh = new Mesh(newMesh);

    loadedMeshes[filepath] = storedMesh;
    return storedMesh;
}

Mesh* MeshManager::GetMesh(const std::string& filepath)
{
    return LoadMesh(filepath);
}
