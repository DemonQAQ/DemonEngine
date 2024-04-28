//
// Created by Demon on 2023/11/7.
//
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <runtime/base/RuntimeApplication.hpp>
#include <core/event/base/event/input/MouseMoveEvent.hpp>
#include <core/event/base/event/os/WindowSizeChangeEvent.hpp>
#include <core/event/base/event/input/ScrollEvent.hpp>
#include "GLFWWindowFactory.hpp"

namespace render
{

    void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        runtimeApp.getRenderManager()->setViewport(0, 0, width, height);
        runtimeApp.callEvent(std::make_shared<event::base::WindowSizeChangeEvent>(width, height));
    }

    void mouse_callback(GLFWwindow *window, double xPos, double yPos)
    {
        runtimeApp.callEvent(std::make_shared<event::base::MouseMoveEvent>(xPos, yPos));
    }

    void scroll_callback(GLFWwindow *window, double xOffset, double yOffset)
    {
        runtimeApp.callEvent(std::make_shared<event::base::ScrollEvent>(xOffset, yOffset));
    }

    GLFWwindow *GLFWWindowFactory::createWindow(int width, int height, const char *title)
    {
        initializeGLFW();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (window == nullptr)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return nullptr;
        }
        windows.push_back(window);

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        return window;
    }

    void GLFWWindowFactory::closeWindow(GLFWwindow *window)
    {
        auto it = std::find(windows.begin(), windows.end(), window);
        if (it == windows.end())return;

        glfwDestroyWindow(*it);
        windows.erase(it);
    }

    GLFWWindowFactory::~GLFWWindowFactory()
    {
        for (auto *wnd: windows)
        {
            if (wnd == nullptr)continue;
            glfwDestroyWindow(wnd);
        }
    }

    void GLFWWindowFactory::initializeGLFW()
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

    GLFWWindowFactory::GLFWWindowFactory()
    {}
}