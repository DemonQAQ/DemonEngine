//
// Created by Demon on 2023/12/6.
//

#ifndef DEMONENGINE_OPENGLINITIALIZER_HPP
#define DEMONENGINE_OPENGLINITIALIZER_HPP

#include "GLFW/glfw3.h"

namespace base::render
{
    class OpenGLInitializer
    {
    public:
        static bool initializeOpenGL(GLFWwindow *window);
    };
}

#endif //DEMONENGINE_OPENGLINITIALIZER_HPP
