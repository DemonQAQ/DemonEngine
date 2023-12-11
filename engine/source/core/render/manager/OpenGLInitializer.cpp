//
// Created by Demon on 2023/11/7.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "OpenGLInitializer.hpp"


namespace base::render
{
    bool OpenGLInitializer::initializeOpenGL(GLFWwindow *window)
    {
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return false;
        }
        glEnable(GL_DEPTH_TEST);
        return true;
    }
}