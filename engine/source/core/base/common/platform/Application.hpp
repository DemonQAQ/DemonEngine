//
// Created by Demon on 2023/12/5.
//

#ifndef DEMONENGINE_APPLICATION_HPP
#define DEMONENGINE_APPLICATION_HPP

#include <core/assets/scene/Scene.hpp>
#include <core/event/base/EventBus.hpp>
#include <core/script/pipline/SerialAssemblyScriptPipLine.hpp>
#include "core/base/interface/Interface.hpp"
#include "core/render/manager/RenderManager.hpp"

namespace base
{
    interface Application
    {
    protected:
        std::shared_ptr<render::RenderManager> renderManager;
        std::shared_ptr<assets::scene::Scene> mainScene;
        std::shared_ptr<event::base::EventBus> eventBus;
        std::shared_ptr<script::SerialAssemblyScriptPipLine> serialScriptPipLine;
        std::vector<std::shared_ptr<event::IEventListener>> listenerList;
        static double deltaTime;
    public:
        Application() : listenerList()
        {
            mainScene = nullptr;
            renderManager = std::make_shared<render::RenderManager>();
            eventBus = std::make_shared<event::base::EventBus>();
            serialScriptPipLine = nullptr;
        }

        virtual int onInitialize() = 0;

        virtual int initialize() = 0;

        virtual void onStart() = 0;

        virtual int start() = 0;

        virtual void onStop() = 0;

        virtual bool stop() = 0;

        virtual int loadAssets() = 0;

        virtual void finalize() = 0;

        virtual void unloadAssets() = 0;

        virtual void tick() = 0;

        virtual bool isQuit() = 0;

        virtual void onInput() = 0;         // 输入处理

        virtual void onUpdate() = 0;        // 更新游戏逻辑

        virtual void onPhysicsUpdate() = 0; // 更新物理

        virtual void onPreRender() = 0;     // 渲染前的准备

        virtual void onRender() = 0;        // 渲染时

        virtual void onPostProcess() = 0;   // 后处理

        virtual void onPostRender() = 0;    // 渲染后的操作

        bool submitScript(std::shared_ptr<script::IScriptEntity> &scriptEntity)
        {
            return serialScriptPipLine->submitScript(scriptEntity);
        }

        void removeScript(std::shared_ptr<UUID> &uuid)
        {
            serialScriptPipLine->removeScript(uuid);
        }

        std::shared_ptr<render::RenderManager> &getRenderManager()
        {
            return renderManager;
        }

        void callEvent(const std::shared_ptr<event::IEvent> &event)
        {
            eventBus->callEvent(event);
        }

        void subscribe(const std::shared_ptr<event::IEventListener> &listener)
        {
            eventBus->subscribe(listener);
            listenerList.push_back(listener);
        }

        static double getDeltaTime()
        {
            return deltaTime;
        }
    };
}


#endif //DEMONENGINE_APPLICATION_HPP
