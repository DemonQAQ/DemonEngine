//
// Created by Demon on 2024/3/25.
//

#ifndef DEMONENGINE_OPENGLAPPLICATION_HPP
#define DEMONENGINE_OPENGLAPPLICATION_HPP


#include "core/base/interface/Interface.hpp"
#include "core/render/manager/WindowFactory.hpp"
#include "Application.hpp"
#include "core/assets/scene/Scene.hpp"

namespace base
{
    class OpenglApplication : implements Application
    {
    private:
        render::WindowFactory windowFactory;
        GLFWwindow *mainWindow{};
        std::shared_ptr<assets::scene::Scene> mainScene;
    public:
        OpenglApplication()
        {
            renderManager = std::make_shared<render::RenderManager>();
        }

        int start() override;

        bool stop() override;

    protected:
        int initialize() override;

        void finalize() override;

        int loadAssets() override;

        void unloadAssets() override;

        void tick() override;

        bool isQuit() override;

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

#endif //DEMONENGINE_OPENGLAPPLICATION_HPP
