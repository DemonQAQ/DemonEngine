//
// Created by Demon on 2023/11/7.
//
#ifndef DEMONENGINE_WINDOWFACTORY_HPP
#define DEMONENGINE_WINDOWFACTORY_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <functional>

namespace base::render
{

    void framebuffer_size_callback(GLFWwindow *window, int width, int height);

    void processInput(GLFWwindow *window);

    void mouse_callback(GLFWwindow *window, double xPos, double yPos);

    void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);

    void processInput(GLFWwindow *window);

    struct CameraParameters
    {
        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        float yaw = -90.0f;
        float pitch = 0.0f;
        float fov = 45.0f;
    };

    struct WindowCallbacks
    {
        ::std::function<void(GLFWwindow *, int, int)> framebufferSizeCallback = framebuffer_size_callback;
        std::function<void(GLFWwindow *, double, double)> cursorPosCallback = mouse_callback;
        std::function<void(GLFWwindow *, double, double)> scrollCallback = scroll_callback;
    };

    struct WindowState
    {
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);  // 默认位置
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // 默认朝向
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);    // 默认上方向
        float yaw = -90.0f;   // 默认偏航角
        float pitch = 0.0f;   // 默认俯仰角
        float fov = 45.0f;    // 默认视野
        float lastX = 800.0f / 2.0;  // 假设窗口宽度为800
        float lastY = 600.0f / 2.0;  // 假设窗口高度为600
        bool firstMouse = true;      // 初始鼠标状态
        float deltaTime = 0.0f;      // 初始时间差
    };

    struct WindowData
    {
        WindowState state;
        WindowCallbacks callbacks;
    };

    class WindowFactory
    {
    private:
        std::vector<GLFWwindow *> windows;
        std::unordered_map<GLFWwindow *, WindowData *> windowDataMap;

        static void initializeGLFW();

    public:
        ~WindowFactory(); // 析构函数
        void closeWindow(GLFWwindow *window);

        GLFWwindow *createWindow(int width, int height, const char *title,
                                 const WindowCallbacks &callbacks = WindowCallbacks(),
                                 const CameraParameters &cameraParams = CameraParameters());

        WindowFactory(const WindowFactory &) = delete;

        WindowFactory &operator=(const WindowFactory &) = delete;

        WindowFactory(WindowFactory &&) = delete;

        WindowFactory();

        WindowFactory &operator=(WindowFactory &&) = delete;
    };

#endif //DEMONENGINE_WINDOWFACTORY_HPP
}