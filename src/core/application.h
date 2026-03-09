#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "config.h"

class application
{
public:
	application(const Resolution& res, const char* title);
	~application();
	void Run();

private:
	GLFWwindow* window;
	unsigned int SCR_WIDTH, SCR_HEIGHT;

	void initGLFW();
	void initGLAD();
	void setupCallbacks();
};

