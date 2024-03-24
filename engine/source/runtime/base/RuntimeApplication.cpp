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
        return OpenglApplication::loadAssets();
    }

    void RuntimeApplication::unloadAssets()
    {
        OpenglApplication::unloadAssets();
    }

    void RuntimeApplication::onStart()
    {
        OpenglApplication::onStart();
    }

    void RuntimeApplication::onStop()
    {
        OpenglApplication::onStop();
    }

    void RuntimeApplication::onRender()
    {
        OpenglApplication::onRender();
    }

    void RuntimeApplication::onInput()
    {
        OpenglApplication::onInput();
    }

    void RuntimeApplication::onUpdate()
    {
        OpenglApplication::onUpdate();
    }

    void RuntimeApplication::onPhysicsUpdate()
    {
        OpenglApplication::onPhysicsUpdate();
    }

    void RuntimeApplication::onPreRender()
    {
        OpenglApplication::onPreRender();
    }

    void RuntimeApplication::onPostProcess()
    {
        OpenglApplication::onPostProcess();
    }

    void RuntimeApplication::onPostRender()
    {
        OpenglApplication::onPostRender();
    }
}