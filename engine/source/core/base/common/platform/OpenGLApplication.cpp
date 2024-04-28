//
// Created by Demon on 2023/12/5.
//
#include <exception>
#include <iostream>
#include <core/assets/manager/AssetsDataMainManager.hpp>
#include <core/render/pipeline/ProgrammablePipeline.hpp>
#include <thread>
#include <chrono>
#include <core/event/base/listener/OSInputEventListener.hpp>
#include "GLFW/glfw3.h"
#include "OpenGLApplication.hpp"

double base::OpenGLApplication::lastFrameTime = 0;
int base::OpenGLApplication::width = 960;
int base::OpenGLApplication::height = 540;

int base::OpenGLApplication::start()
{
    try
    {
        onStart();

        int i = 0;
        auto next_tick = std::chrono::steady_clock::now();

        while (!isQuit())
        {
            auto start_time = std::chrono::steady_clock::now();
            std::this_thread::sleep_until(next_tick);

            tick();
            auto end_time = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> frame_duration = end_time - start_time;

            std::cerr << "Frame " << i << ": Duration = " << frame_duration.count() << " ms" << std::endl;
            i++;

            next_tick += std::chrono::milliseconds(16);
            if (end_time > next_tick)
            {
                std::cerr << "Warning: Frame processing exceeded 16 ms" << std::endl;
            }
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

bool base::OpenGLApplication::stop()
{
    onStop();
    unloadAssets();
    finalize();
    return false;
}

int base::OpenGLApplication::initialize()
{
    assets::AssetsDataMainManager::initialize();
    // 创建窗口
    mainWindow = windowFactory.createWindow(width, height, "Demon Engine");

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

    onInitialize();
    return 0;
}

void base::OpenGLApplication::finalize()
{
    if (mainWindow)
    {
        glfwDestroyWindow(mainWindow);
    }
    glfwTerminate();
}

int base::OpenGLApplication::loadAssets()
{
    return 0;
}

void base::OpenGLApplication::unloadAssets()
{

}

void base::OpenGLApplication::tick()
{
    double currentFrameTime = glfwGetTime();
    deltaTime = currentFrameTime - lastFrameTime;
    lastFrameTime = currentFrameTime;

    glfwPollEvents();
    onInput();       // 处理输入
    eventBus->callBusEvents();
    onUpdate();      // 更新游戏逻辑
    onPhysicsUpdate(); // 更新物理
    onPreRender();   // 渲染前的准备
    onRender();      // 执行渲染
    onPostProcess(); // 后处理
    onPostRender();  // 渲染后的操作
    glfwSwapBuffers(mainWindow);
}

bool base::OpenGLApplication::isQuit()
{
    return glfwWindowShouldClose(mainWindow);
}

void base::OpenGLApplication::onStop()
{

}

void base::OpenGLApplication::onStart()
{
    lastFrameTime = glfwGetTime();
}

void base::OpenGLApplication::onRender()
{
    renderManager->render();
}

void base::OpenGLApplication::processInput()
{
    GLFWwindow *window = mainWindow;
    static bool lastStateW = false;
    static bool lastStateS = false;
    static bool lastStateA = false;
    static bool lastStateD = false;

    bool currentStateW = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    bool currentStateS = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    bool currentStateA = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    bool currentStateD = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Handle Key W
    if (currentStateW && !lastStateW)
        callEvent(std::make_shared<event::base::KeyDownEvent>(event::base::KeyType::KEY_W));
    else if (!currentStateW && lastStateW)
        callEvent(std::make_shared<event::base::KeyUpEvent>(event::base::KeyType::KEY_W));
    lastStateW = currentStateW;

    // Handle Key S
    if (currentStateS && !lastStateS)
        callEvent(std::make_shared<event::base::KeyDownEvent>(event::base::KeyType::KEY_S));
    else if (!currentStateS && lastStateS)
        callEvent(std::make_shared<event::base::KeyUpEvent>(event::base::KeyType::KEY_S));
    lastStateS = currentStateS;

    // Handle Key A
    if (currentStateA && !lastStateA)
        callEvent(std::make_shared<event::base::KeyDownEvent>(event::base::KeyType::KEY_A));
    else if (!currentStateA && lastStateA)
        callEvent(std::make_shared<event::base::KeyUpEvent>(event::base::KeyType::KEY_A));
    lastStateA = currentStateA;

    // Handle Key D
    if (currentStateD && !lastStateD)
        callEvent(std::make_shared<event::base::KeyDownEvent>(event::base::KeyType::KEY_D));
    else if (!currentStateD && lastStateD)
        callEvent(std::make_shared<event::base::KeyUpEvent>(event::base::KeyType::KEY_D));
    lastStateD = currentStateD;
}

void base::OpenGLApplication::onInput()
{
    processInput();
}

void base::OpenGLApplication::onUpdate()
{
    serialScriptPipLine->onUpdate();
    mainScene->update();
    renderManager->updateCameraInfo(mainScene->getMainCameraEntity());
}

void base::OpenGLApplication::onPhysicsUpdate()
{
    serialScriptPipLine->onPhysics();
}

void base::OpenGLApplication::onPreRender()
{
    this->mainScene->beforeRendering({});
    renderManager->updateCameraInfo(this->mainScene->getMainCameraEntity());
}

void base::OpenGLApplication::onPostProcess()
{

}

void base::OpenGLApplication::onPostRender()
{
    this->mainScene->afterRendering({});
}

void OpenGLApplication::loadScene(const std::shared_ptr<assets::scene::Scene> &scene)
{
    this->mainScene = scene;
}

std::shared_ptr<assets::scene::Scene> OpenGLApplication::getScene()
{
    return mainScene;
}

int OpenGLApplication::getScreenWidth()
{
    return width;
}

int OpenGLApplication::getScreenHeight()
{
    return height;
}

int OpenGLApplication::onInitialize()
{
    subscribe(std::make_shared<event::base::OSInputEventListener>());
    return 0;
}


