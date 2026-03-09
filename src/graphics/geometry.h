#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
float* getCubeVertices();
int getCubeVertexCount();

float* getQuadVertices();
int getQuadVertexCount();

struct Vertex
{
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
};
