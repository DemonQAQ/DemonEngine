//
// Created by Demon on 2024/5/1.
//

#ifndef DEMONENGINE_SCRIPTINITIALIZER_HPP
#define DEMONENGINE_SCRIPTINITIALIZER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/interface/Initializer.hpp"
#include "mono/metadata/assembly.h"
#include "mono/utils/mono-forward.h"
#include "mono/jit/jit.h"

static char const * monoLibraryPath = MONO_LIBRARY_PATH;
static char const * monoEtcPath = MONO_ETC_PATH;

namespace script
{
    class ScriptInitializer : implements base::Initializer
    {
    public:
        bool init(const std::vector<std::any> &params) override
        {
            if (isInit())return false;

            initMono();

            setInit(true);
            return true;
        }
    private:
        static void initMono()
        {
            mono_set_dirs(monoLibraryPath, monoEtcPath);
        }
    };
}

#endif //DEMONENGINE_SCRIPTINITIALIZER_HPP
