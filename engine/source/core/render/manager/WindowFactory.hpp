//
// Created by Demon on 2023/11/7.
//
#ifndef DEMONENGINE_WINDOWFACTORY_HPP
#define DEMONENGINE_WINDOWFACTORY_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <functional>

namespace render
{
    void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    void mouse_callback(GLFWwindow *window, double xPos, double yPos);

    void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

    class WindowFactory
    {
    private:
        std::vector<GLFWwindow *> windows;
        static void initializeGLFW();

    public:
        ~WindowFactory(); // 析构函数
        void closeWindow(GLFWwindow *window);

        GLFWwindow *createWindow(int width, int height, const char *title);

        WindowFactory(const WindowFactory &) = delete;

        WindowFactory &operator=(const WindowFactory &) = delete;

        WindowFactory(WindowFactory &&) = delete;

        WindowFactory();

        WindowFactory &operator=(WindowFactory &&) = delete;
    };
}
#endif //DEMONENGINE_WINDOWFACTORY_HPP