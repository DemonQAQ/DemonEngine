//
// Created by Demon on 2024/4/26.
//

#ifndef DEMONENGINE_SCRIPTLOADER_HPP
#define DEMONENGINE_SCRIPTLOADER_HPP

#include <string>
#include <memory>
#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileLoader.hpp"
#include "core/script/entity/AssemblyScriptEntity.hpp"

namespace assets
{
    class ScriptLoader: implements IFileLoader
    {
    public:
        static std::shared_ptr<script::AssemblyScriptEntity> loadScript(const std::string& name);
    };
}


#endif //DEMONENGINE_SCRIPTLOADER_HPP
