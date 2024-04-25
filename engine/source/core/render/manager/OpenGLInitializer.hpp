//
// Created by Demon on 2023/12/6.
//

#ifndef DEMONENGINE_OPENGLINITIALIZER_HPP
#define DEMONENGINE_OPENGLINITIALIZER_HPP

#include "core/base/lib/pch.hpp"
#include "GLFW/glfw3.h"
#include "core/base/interface/Initializer.hpp"
#include "iostream"


namespace render
{
    class OpenGLInitializer : implements base::Initializer
    {
    public:
        bool init(const std::vector<std::any> &params) override
        {
            if (isInit())return true;
            if (params.empty()) return false;
            GLFWwindow *window;
            try
            {
                window = std::any_cast<GLFWwindow *>(params[0]);
            }
            catch (const std::bad_any_cast &e)
            {
                std::cerr << "Initialization failed: " << e.what() << std::endl;
                return false;
            }

            bool result = initializeOpenGL(window);
            setInit(result);
            return result;
        }

    private:
        static bool initializeOpenGL(GLFWwindow *window)
        {
            glfwMakeContextCurrent(window);
            if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
            {
                std::cerr << "Failed to initialize GLAD" << std::endl;
                return false;
            }
            glEnable(GL_DEPTH_TEST);
            printOpenGLInfo();
            return true;
        }

        static void printOpenGLInfo()
        {
            const GLubyte* renderer = glGetString(GL_RENDERER);  // 获取渲染器标识符
            const GLubyte* vendor = glGetString(GL_VENDOR);      // 获取供应商名
            const GLubyte* version = glGetString(GL_VERSION);   // 获取OpenGL版本
            std::cout << "Renderer: " << renderer << std::endl;
            std::cout << "Vendor: " << vendor << std::endl;
            std::cout << "OpenGL version supported: " << version << std::endl;
        }
    };
}

#endif //DEMONENGINE_OPENGLINITIALIZER_HPP
