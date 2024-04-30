//
// Created by Demon on 2024/5/1.
//

#ifndef DEMONENGINE_MONOSCRIPTTHREAD_HPP
#define DEMONENGINE_MONOSCRIPTTHREAD_HPP

#include <memory>
#include "core/base/interface/Interface.hpp"
#include "core/script/interface/IScriptRuntimeThread.hpp"
#include "mono/utils/mono-forward.h"
#include <boost/thread.hpp>

namespace script
{
    class MonoScriptThread : implements IScriptRuntimeThread
    {
    private:
        MonoDomain *domain = nullptr;
        std::shared_ptr<boost::thread> thread;
    public:
        MonoScriptThread();

        ~MonoScriptThread() override;

        void start() override;

        void stop() override;

        [[nodiscard]] bool isRunning() const override;

        void initializeDomain(const std::vector<std::any> &params) override;

        void destroyDomain() override;
    };
}

#endif //DEMONENGINE_MONOSCRIPTTHREAD_HPP
