//
// Created by Demon on 2024/5/1.
//

#ifndef DEMONENGINE_ISCRIPTRUNTIMETHREAD_HPP
#define DEMONENGINE_ISCRIPTRUNTIMETHREAD_HPP

#include <vector>
#include <any>
#include "core/base/interface/Interface.hpp"

namespace script
{
    interface IScriptRuntimeThread
    {
        virtual ~IScriptRuntimeThread() = default;

        virtual void start() = 0;

        virtual void stop() = 0;

        [[nodiscard]] virtual bool isRunning() const = 0;

        virtual void initializeDomain(const std::vector<std::any> &params) = 0;

        virtual void destroyDomain() = 0;
    };
};

#endif //DEMONENGINE_ISCRIPTRUNTIMETHREAD_HPP