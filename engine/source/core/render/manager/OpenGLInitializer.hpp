//
// Created by Demon on 2023/12/6.
//

#ifndef DEMONENGINE_OPENGLINITIALIZER_HPP
#define DEMONENGINE_OPENGLINITIALIZER_HPP

#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Initializer.hpp"
#include "iostream"


namespace render
{
    class OpenGLInitializer : implements Initializer
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

            return true;
        }
    };
}

#endif //DEMONENGINE_OPENGLINITIALIZER_HPP
