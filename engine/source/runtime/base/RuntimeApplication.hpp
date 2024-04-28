// RuntimeApplication.hpp

#ifndef DEMONENGINE_RUNTIMEAPPLICATION_HPP
#define DEMONENGINE_RUNTIMEAPPLICATION_HPP

#include "../../core/base/interface/Interface.hpp"
#include "GLFW/glfw3.h"
#include "core/base/common/platform/Application.hpp"
#include "core/base/common/platform/OpenGLApplication.hpp"

using namespace base;
using namespace render;

namespace runtime
{
    class RuntimeApplication : implements OpenGLApplication
    {

    public:
        RuntimeApplication();

        int loadAssets() override;

        void unloadAssets() override;

        void onStart() override;

        void onStop() override;

        void onRender() override;

        void onInput() override;

        void onUpdate() override;

        void onPhysicsUpdate() override;

        void onPreRender() override;

        void onPostProcess() override;

        void onPostRender() override;
    };
}

extern runtime::RuntimeApplication runtimeApp;

#endif // DEMONENGINE_RUNTIMEAPPLICATION_HPP