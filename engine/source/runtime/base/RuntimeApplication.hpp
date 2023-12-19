// RuntimeApplication.hpp

#ifndef DEMONENGINE_RUNTIMEAPPLICATION_HPP
#define DEMONENGINE_RUNTIMEAPPLICATION_HPP

#include "../../core/base/interface/Interface.hpp"
#include "GLFW/glfw3.h"
#include "../../core/base/interface/Application.hpp"

using namespace base;
using namespace base::render;

namespace runtime
{
    class RuntimeApplication : implements BaseApplication
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

#endif // DEMONENGINE_RUNTIMEAPPLICATION_HPP