//
// Created by Demon on 2023/12/5.
//
#include <exception>
#include <iostream>
#include "Application.hpp"
#include "core/render/OpenGLInitializer.hpp"

int base::BaseApplication::start()
{
    try
    {
        if (initialize() != 0)
        {
            return -1;
        }

        if (loadAssets() != 0)
        {
            return -1;
        }

        onStart();

        while (!isQuit())
        {
            tick();
        }

        stop();
        return 0;
    } catch (const std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    } catch (...)
    {
        std::cerr << "Unknown exception caught" << std::endl;
    }
    return 0;
}

bool base::BaseApplication::stop()
{
    onStop();
    unloadAssets();
    finalize();
    return false;
}

int base::BaseApplication::initialize()
{
    // 创建窗口
    mainWindow = windowFactory.createWindow(800, 600, "Demo Application");

    // 如果窗口创建失败，返回错误代码
    if (!mainWindow)
    {
        return -1;
    }

    // 初始化OpenGL
    if (!base::render::OpenGLInitializer::initializeOpenGL(mainWindow))
    {
        return -1;
    }

    // 设置用户指针，以便在回调函数中访问应用程序状态
    glfwSetWindowUserPointer(mainWindow, this);
    return 0;
}

void base::BaseApplication::finalize()
{
    if (mainWindow)
    {
        glfwDestroyWindow(mainWindow);
        mainWindow = nullptr;
    }
    glfwTerminate();
}

int base::BaseApplication::loadAssets()
{
    return 0;
}

void base::BaseApplication::unloadAssets()
{

}

void base::BaseApplication::tick()
{
    glfwPollEvents();
    onInput();       // 处理输入
    onUpdate();      // 更新游戏逻辑
    onPhysicsUpdate(); // 更新物理
    onPreRender();   // 渲染前的准备
    onRender();      // 执行渲染
    onPostProcess(); // 后处理
    onPostRender();  // 渲染后的操作
    glfwSwapBuffers(mainWindow);
}

bool base::BaseApplication::isQuit()
{
    return glfwWindowShouldClose(mainWindow);
}

void base::BaseApplication::onStop()
{

}

void base::BaseApplication::onStart()
{

}

void base::BaseApplication::onRender()
{

}

void base::BaseApplication::onInput()
{
    if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mainWindow, true);
}

void base::BaseApplication::onUpdate()
{

}

void base::BaseApplication::onPhysicsUpdate()
{

}

void base::BaseApplication::onPreRender()
{

}

void base::BaseApplication::onPostProcess()
{

}

void base::BaseApplication::onPostRender()
{

}


