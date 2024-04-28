//
// Created by Demon on 2024/3/25.
//

#ifndef DEMONENGINE_OPENGLAPPLICATION_HPP
#define DEMONENGINE_OPENGLAPPLICATION_HPP


#include "core/base/interface/Interface.hpp"
#include "core/render/manager/GLFWWindowFactory.hpp"
#include "Application.hpp"
#include "core/assets/scene/Scene.hpp"

namespace base
{
    class OpenGLApplication : implements Application
    {
    private:
        render::GLFWWindowFactory windowFactory;
        GLFWwindow *mainWindow;
        static double lastFrameTime;
        static int width;
        static int height;
    public:
        OpenGLApplication() : Application()
        {
            mainWindow = nullptr;
        }

        static int getScreenWidth();

        static int getScreenHeight();

        int onInitialize() override;

        int start() override;

        bool stop() override;

        void loadScene(const std::shared_ptr<assets::scene::Scene> &scene);

        std::shared_ptr<assets::scene::Scene> getScene();

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
    private:
        void processInput();
    };
}

#endif //DEMONENGINE_OPENGLAPPLICATION_HPP
