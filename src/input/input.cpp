#define GLM_ENABLE_EXPERIMENTAL
#include "imgui.h"
#include "imgui_internal.h"
#include "input.h"
#include "../core/camera.h"
#include <glad/glad.h>
#include "MessageManager.h"
#include "MessageQueue.h"

extern GLFWwindow* window;

bool bWireFramez = false;
bool bEnableCRT = false;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    static bool tabPressed = false;
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !tabPressed)
    {
        bEnableCRT = !bEnableCRT;
        tabPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
    {
        tabPressed = false;
    }

    static bool wPressed = false;
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && !wPressed)
    {
        bWireFramez = !bWireFramez;
        glPolygonMode(GL_FRONT_AND_BACK, bWireFramez ? GL_LINE : GL_FILL);
        wPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_RELEASE)
    {
        wPressed = false;
    }
}

void camera::Inputs(GLFWwindow* window, float deltaTime, bool bAllowTranslations)
{
	//Mouse message processing
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    bool bIsMouseClicked = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse || io.WantCaptureKeyboard)
    {
        return;
    }
	//Process mouse message
    // MouseMessage mouseMsg(static_cast<int>(mouseX), static_cast<int>(mouseY), bIsMouseClicked);
    g_messageQueue.push(std::make_unique<MouseMessage>(static_cast<int>(mouseX), static_cast<int>(mouseY), bIsMouseClicked));

    float frameSpeed = speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        frameSpeed *= sprintMultiplier;

    if (bAllowTranslations)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            Position += frameSpeed * deltaTime * Orientation;
            //std::cout << "w pressed" << std::endl;
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            Position += frameSpeed * deltaTime * -glm::normalize(glm::cross(Orientation, Up));
            //std::cout << "a pressed" << std::endl;
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            Position += frameSpeed * deltaTime * -Orientation;
            //std::cout << "s pressed" << std::endl;
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            Position += frameSpeed * deltaTime * glm::normalize(glm::cross(Orientation, Up));
            //std::cout << "d pressed" << std::endl;
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            Position += frameSpeed * deltaTime * Up;
            //std::cout << "SPACE PRESSED" << std::endl;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            Position += frameSpeed * deltaTime * -Up;
        }
    }

    

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        frameSpeed = 0.3f;
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        frameSpeed = 0.1f;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevents camera from jumping on the first clickz
        if (bFirstClick)
        {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            bFirstClick = false;
        }

        double mouseX;
        double mouseY;

        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
        // and then "transforms" them into degrees 
        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

        // Calculates upcoming vertical change in the Orientation
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        // Decides whether or not the next vertical Orientation is legal or not
        if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            Orientation = newOrientation;
        }

        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        bFirstClick = true;
    }
}