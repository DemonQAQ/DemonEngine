//
// Created by Demon on 2023/12/6.
//
#include "RuntimeApplication.hpp"
#include <exception>
#include <iostream>

using namespace base;
using namespace render;

namespace runtime
{
    RuntimeApplication::RuntimeApplication()
    {

    }

    int RuntimeApplication::loadAssets()
    {
        return BaseApplication::loadAssets();
    }

    void RuntimeApplication::unloadAssets()
    {
        BaseApplication::unloadAssets();
    }

    void RuntimeApplication::onStart()
    {
        BaseApplication::onStart();
    }

    void RuntimeApplication::onStop()
    {
        BaseApplication::onStop();
    }

    void RuntimeApplication::onRender()
    {
        BaseApplication::onRender();
    }

    void RuntimeApplication::onInput()
    {
        BaseApplication::onInput();
    }

    void RuntimeApplication::onUpdate()
    {
        BaseApplication::onUpdate();
    }

    void RuntimeApplication::onPhysicsUpdate()
    {
        BaseApplication::onPhysicsUpdate();
    }

    void RuntimeApplication::onPreRender()
    {
        BaseApplication::onPreRender();
    }

    void RuntimeApplication::onPostProcess()
    {
        BaseApplication::onPostProcess();
    }

    void RuntimeApplication::onPostRender()
    {
        BaseApplication::onPostRender();
    }
}