//
// Created by Demon on 2023/12/5.
//

#ifndef DEMONENGINE_APPLICATION_HPP
#define DEMONENGINE_APPLICATION_HPP

#include "core/base/interface/Interface.hpp"
#include "core/render/manager/RenderManager.hpp"

namespace base
{
    interface Application
    {
    protected:
        std::shared_ptr<render::RenderManager> renderManager;
    public:
        virtual ~Application() = default;

        virtual int start() = 0;

        virtual bool stop() = 0;

        virtual int initialize() = 0;

        virtual int loadAssets() = 0;

        virtual void finalize() = 0;

        virtual void unloadAssets() = 0;

        virtual void tick() = 0;

        virtual bool isQuit() = 0;

        virtual void onStart() = 0;

        virtual void onStop() = 0;

        virtual void onInput() = 0;         // 输入处理

        virtual void onUpdate() = 0;        // 更新游戏逻辑

        virtual void onPhysicsUpdate() = 0; // 更新物理

        virtual void onPreRender() = 0;     // 渲染前的准备

        virtual void onRender() = 0;        // 渲染时

        virtual void onPostProcess() = 0;   // 后处理

        virtual void onPostRender() = 0;    // 渲染后的操作

    };
}


#endif //DEMONENGINE_APPLICATION_HPP
