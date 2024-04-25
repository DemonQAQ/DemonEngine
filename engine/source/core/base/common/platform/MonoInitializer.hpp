//
// Created by Demon on 2024/4/26.
//

#ifndef DEMONENGINE_MONOINITIALIZER_HPP
#define DEMONENGINE_MONOINITIALIZER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/interface/Initializer.hpp"
#include "mono/jit/jit.h"

namespace base
{
    class MonoInitializer : implements Initializer
    {
        bool init(const std::vector<std::any> &params) override
        {
            if (isInit())return true;

            mono_jit_init("GameScriptingDomain");

            setInit(true);
        }
    };
}

#endif //DEMONENGINE_MONOINITIALIZER_HPP
