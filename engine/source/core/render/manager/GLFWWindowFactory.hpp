//
// Created by Demon on 2023/11/7.
//
#ifndef DEMONENGINE_GLFWWINDOWFACTORY_HPP
#define DEMONENGINE_GLFWWINDOWFACTORY_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <functional>

namespace render
{
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    void mouse_callback(GLFWwindow *window, double xPos, double yPos);

    void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

    class GLFWWindowFactory
    {
    private:
        std::vector<GLFWwindow *> windows;
        static void initializeGLFW();

    public:
        ~GLFWWindowFactory(); // 析构函数
        void closeWindow(GLFWwindow *window);

        GLFWwindow *createWindow(int width, int height, const char *title);

        GLFWWindowFactory(const GLFWWindowFactory &) = delete;

        GLFWWindowFactory &operator=(const GLFWWindowFactory &) = delete;

        GLFWWindowFactory(GLFWWindowFactory &&) = delete;

        GLFWWindowFactory();

        GLFWWindowFactory &operator=(GLFWWindowFactory &&) = delete;
    };
}
#endif //DEMONENGINE_GLFWWINDOWFACTORY_HPP