#pragma once
#include "../shaders/shader.h"
#include "geometry.h"
#include <glm/glm.hpp>
#include <vector>
#include "../core/camera.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include "../physics/physicsAABB.h"
#include "../physics/physicsManager.h"



struct CubeTransform //Entity transform data
{
    std::string name = "Nameless Cube";
    glm::vec3 position{ 0.0f };
    glm::vec3 rotation{ 0.0f };
    glm::vec3 scale{ 1.0f };

	Physics::PhysicsComponent physics;
	Physics::CollisionType collisionType = Physics::CollisionType::OBSTACLE;

	Physics::AABB GetWorldAABB() const
	{
		return Physics::AABB::FromTransform(position, scale);
	}
	
	std::string meshPath;
	unsigned int textureID = 0;
	std::string texturePath;
	unsigned int specularMapID = 0;
	std::string specularMapPath;
	unsigned int VAO;    
};

class BasicShadowMapper;

class renderer
{
public:
    renderer(unsigned int width, unsigned int height);
    ~renderer();
    void render(const std::vector<CubeTransform>& cubes, camera& cam);

	Shader* mainShader = nullptr;

private:
	BasicShadowMapper* shadowMapper = nullptr;
	
    unsigned int SCR_WIDTH, SCR_HEIGHT;
    unsigned int VAO = 0, VBO = 0, quadVAO = 0, quadVBO = 0;
    unsigned int framebuffer = 0, texColorBuffer = 0, RBO = 0;
    unsigned int texture1 = 0;

    void setupCube();
    void setupQuad();
    void setupFramebuffer();
    void loadTextures();

	void DrawMesh();
};
	class Mesh
	{
	public:
		unsigned int VAO, VBO;
		int vertexCount;

		Mesh(const std::vector<float>& vertices)
		{
			vertexCount = vertices.size() / 8;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
			
			constexpr GLsizei stride = sizeof(float) * 8;

			// position
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
			glEnableVertexAttribArray(0);

			// texcoords/uv
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));
			glEnableVertexAttribArray(1);

			// normal
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 5));
			glEnableVertexAttribArray(2);

			glBindVertexArray(0);
		}

		void DrawMesh()
		{
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
			glBindVertexArray(0);
		}
};

