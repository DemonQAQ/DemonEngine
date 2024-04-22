//
// Created by Demon on 2023/12/5.
//
#include <exception>
#include <iostream>
#include <core/assets/manager/AssetsDataMainManager.hpp>
#include <core/render/pipeline/ProgrammablePipeline.hpp>
#include <thread>
#include <chrono>
#include "GLFW/glfw3.h"
#include "OpenglApplication.hpp"

int base::OpenglApplication::start()
{
    try
    {
        onStart();

        int i = 0;
        auto next_tick = std::chrono::steady_clock::now();

        while (!isQuit())
        {
            std::this_thread::sleep_until(next_tick);

            //todo
            std::cerr << "第" << i << "次tick准备" << std::endl;
            tick();
            std::cerr << "第" << i << "次tick结束" << std::endl;
            i++;

            next_tick += std::chrono::milliseconds(200);
        }

        stop();
        return 0;
    } catch (const std::exception &e)
    {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    } catch (...)
    {
        std::cerr << "UNKNOWN exception caught" << std::endl;
    }
    return 0;
}

bool base::OpenglApplication::stop()
{
    onStop();
    unloadAssets();
    finalize();
    return false;
}

int base::OpenglApplication::initialize()
{
    assets::AssetsDataMainManager::initialize();
// 创建窗口
    mainWindow = windowFactory.createWindow(800, 600, "Demo Application");

    // 如果窗口创建失败，返回错误代码
    if (!mainWindow)
    {
        return -1;
    }

    // 初始化OpenGL
    if (!renderManager->init({render::RenderApiType::OpenGL, mainWindow, render::PipelineType::PROGRAMMABLE}))
    {
        return -1;
    }

    // 设置用户指针，以便在回调函数中访问应用程序状态
    glfwSetWindowUserPointer(mainWindow, this);
    return 0;
}

void base::OpenglApplication::finalize()
{
    if (mainWindow)
    {
        glfwDestroyWindow(mainWindow);
    }
    glfwTerminate();
}

int base::OpenglApplication::loadAssets()
{
    return 0;
}

void base::OpenglApplication::unloadAssets()
{

}

void base::OpenglApplication::tick()
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

bool base::OpenglApplication::isQuit()
{
    return glfwWindowShouldClose(mainWindow);
}

void base::OpenglApplication::onStop()
{

}

void base::OpenglApplication::onStart()
{

}

void base::OpenglApplication::onRender()
{
    renderManager->render();
}

void base::OpenglApplication::onInput()
{
    if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mainWindow, true);
}

void base::OpenglApplication::onUpdate()
{

}

void base::OpenglApplication::onPhysicsUpdate()
{

}

void base::OpenglApplication::onPreRender()
{
    this->mainScene->beforeRendering({});
}

void base::OpenglApplication::onPostProcess()
{

}

void base::OpenglApplication::onPostRender()
{
    this->mainScene->afterRendering({});
}

void OpenglApplication::loadScene(const std::shared_ptr<assets::scene::Scene> &scene)
{
    this->mainScene = scene;
}

std::shared_ptr<assets::scene::Scene> OpenglApplication::getScene()
{
    return mainScene;
}


