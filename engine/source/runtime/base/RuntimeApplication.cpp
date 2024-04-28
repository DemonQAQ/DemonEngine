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
        return OpenGLApplication::loadAssets();
    }

    void RuntimeApplication::unloadAssets()
    {
        OpenGLApplication::unloadAssets();
    }

    void RuntimeApplication::onStart()
    {
        OpenGLApplication::onStart();
    }

    void RuntimeApplication::onStop()
    {
        OpenGLApplication::onStop();
    }

    void RuntimeApplication::onRender()
    {
        OpenGLApplication::onRender();
    }

    void RuntimeApplication::onInput()
    {
        OpenGLApplication::onInput();
    }

    void RuntimeApplication::onUpdate()
    {
        OpenGLApplication::onUpdate();
    }

    void RuntimeApplication::onPhysicsUpdate()
    {
        OpenGLApplication::onPhysicsUpdate();
    }

    void RuntimeApplication::onPreRender()
    {
        OpenGLApplication::onPreRender();
    }

    void RuntimeApplication::onPostProcess()
    {
        OpenGLApplication::onPostProcess();
    }

    void RuntimeApplication::onPostRender()
    {
        OpenGLApplication::onPostRender();
    }
}