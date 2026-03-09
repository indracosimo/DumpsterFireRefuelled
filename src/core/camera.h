#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
#define GLFW_INCLUDE_NONE
#define GLM_ENABLE_EXPERIMENTAL
#include "../shaders/shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

class camera
{
public:
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix(float FOVdeg, float nearPlane, float farPlane) const;

	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	bool bFirstClick = true;

	int width;
	int height;

	float speed = 2.5f;
	float sensitivity = 100.0f;
	float sprintMultiplier = 3.0f;

	camera(int width, int height, glm::vec3 position);

	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader,  const char* modelUniform, const char* viewUniform, const char* projectionUniform);
	void Inputs(GLFWwindow* window, float deltaTime, bool bAllowTranslation = true);
};

#endif