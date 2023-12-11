//
// Created by Demon on 2023/11/7.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <functional>
#include "WindowFactory.hpp"

namespace base::render
{

    void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void mouse_callback(GLFWwindow *window, double xPos, double yPos)
    {
        auto state = static_cast<WindowState *>(glfwGetWindowUserPointer(window));
        float xPosF = static_cast<float>(xPos);
        float yPosF = static_cast<float>(yPos);

        if (state->firstMouse)
        {
            state->lastX = xPosF;
            state->lastY = yPosF;
            state->firstMouse = false;
        }

        float xOffSet = xPosF - state->lastX;
        float yOffSet = state->lastY - yPosF;
        state->lastX = xPosF;
        state->lastY = yPosF;

        float sensitivity = 0.1f;
        xOffSet *= sensitivity;
        yOffSet *= sensitivity;

        state->yaw += xOffSet;
        state->pitch += yOffSet;

        if (state->pitch > 89.0f)
            state->pitch = 89.0f;
        if (state->pitch < -89.0f)
            state->pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(state->yaw)) * cos(glm::radians(state->pitch));
        front.y = sin(glm::radians(state->pitch));
        front.z = sin(glm::radians(state->yaw)) * cos(glm::radians(state->pitch));
        state->cameraFront = glm::normalize(front);
    }

    void scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
    {
        auto *state = static_cast<WindowState *>(glfwGetWindowUserPointer(window));
        state->fov -= (float) yOffset;
        if (state->fov < 1.0f)
            state->fov = 1.0f;
        if (state->fov > 45.0f)
            state->fov = 45.0f;
    }

    void framebuffer_size_callback_wrapper(GLFWwindow *window, int width, int height)
    {
        auto *windowData = static_cast<WindowData *>(glfwGetWindowUserPointer(window));
        windowData->callbacks.framebufferSizeCallback(window, width, height);
    }

    void mouse_callback_wrapper(GLFWwindow *window, double xPos, double yPos)
    {
        auto *windowData = static_cast<WindowData *>(glfwGetWindowUserPointer(window));
        windowData->callbacks.cursorPosCallback(window, xPos, yPos);
    }

    void scroll_callback_wrapper(GLFWwindow *window, double xOffSet, double yOffSet)
    {
        auto *windowData = static_cast<WindowData *>(glfwGetWindowUserPointer(window));
        windowData->callbacks.scrollCallback(window, xOffSet, yOffSet);
    }

    void processInput(GLFWwindow *window)
    {
        auto *state = static_cast<WindowState *>(glfwGetWindowUserPointer(window));
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        auto cameraSpeed = static_cast<float>(2.5 * state->deltaTime);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            state->cameraPos += cameraSpeed * state->cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            state->cameraPos -= cameraSpeed * state->cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            state->cameraPos -= glm::normalize(glm::cross(state->cameraFront, state->cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            state->cameraPos += glm::normalize(glm::cross(state->cameraFront, state->cameraUp)) * cameraSpeed;
    }

    GLFWwindow *WindowFactory::createWindow(int width, int height, const char *title, const WindowCallbacks &callbacks,
                                            const CameraParameters &cameraParams)
    {
        initializeGLFW();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (window == nullptr)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return nullptr;
        }

        auto *windowData = new WindowData{{
                                                  cameraParams.pos,    // cameraPos
                                                  cameraParams.front,  // cameraFront
                                                  cameraParams.up,     // cameraUp
                                                  cameraParams.yaw,    // yaw
                                                  cameraParams.pitch,  // pitch
                                                  cameraParams.fov,    // fov
                                                  800.0f / 2.0,        // lastX
                                                  600.0 / 2.0,         // lastY
                                                  true,                // firstMouse
                                                  0.0f,                // deltaTime
                                                  // ... initialize other state variables ...
                                          }, callbacks};

        windowDataMap[window] = windowData;
        windows.push_back(window);

        glfwSetWindowUserPointer(window, windowData);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        return window;
    }

    void WindowFactory::closeWindow(GLFWwindow *window)
    {
        auto it = std::find(windows.begin(), windows.end(), window);
        if (it == windows.end())return;

        auto dataIt = windowDataMap.find(window);
        if (dataIt != windowDataMap.end())
        {
            delete dataIt->second;
            windowDataMap.erase(dataIt);
        }

        glfwDestroyWindow(*it);
        windows.erase(it);
    }

    WindowFactory::~WindowFactory()
    {
        for (auto *wnd: windows)
        {
            if (wnd == nullptr)continue;
            auto it = windowDataMap.find(wnd);
            if (it != windowDataMap.end())
            {
                delete it->second;
                windowDataMap.erase(it);
            }
            glfwDestroyWindow(wnd);
        }
    }

    void WindowFactory::initializeGLFW()
    {
        static bool initialized = false;
        if (initialized)return;

        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }
        initialized = true;
    }

    WindowFactory::WindowFactory()
    {}
}