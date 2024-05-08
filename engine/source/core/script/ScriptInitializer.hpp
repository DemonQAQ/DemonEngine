//
// Created by Demon on 2024/5/1.
//

#ifndef DEMONENGINE_SCRIPTINITIALIZER_HPP
#define DEMONENGINE_SCRIPTINITIALIZER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/interface/Initializer.hpp"
#include "mono/metadata/assembly.h"
#include "mono/metadata/appdomain.h"
#include "mono/utils/mono-forward.h"
#include "mono/jit/jit.h"
#include "mono/metadata/mono-config.h"

static char const *monoLibraryPath = MONO_LIBRARY_PATH;
static char const *monoEtcPath = MONO_ETC_PATH;

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

        void finalize() override
        {
            if (isInit())
            {
                std::cout << "Finalizing Mono..." << std::endl;
                setInit(false);
            }
        }

    private:
        static void initMono()
        {
            std::cout << "Setting Mono directories..." << std::endl;
            std::cout << "Library path: " << monoLibraryPath << std::endl;
            std::cout << "Configuration path: " << monoEtcPath << std::endl;
            mono_set_dirs(monoLibraryPath, monoEtcPath);
            mono_config_parse(NULL);
            mono_jit_init("DemonEngineRuntime");
        }
    };
}

#endif //DEMONENGINE_SCRIPTINITIALIZER_HPP
