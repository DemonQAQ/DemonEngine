//
// Created by Demon on 2024/4/26.
//

#ifndef DEMONENGINE_ASYNCSCRIPTPIPLINE_HPP
#define DEMONENGINE_ASYNCSCRIPTPIPLINE_HPP

#include "core/base/interface/Interface.hpp"
#include "core/script/interface/IScriptPipline.hpp"

namespace script
{
    class AsyncScriptPipLine: implements IScriptPipLine
    {
    public:
        bool submitScript(std::shared_ptr<ScriptEntity> &scriptEntity) override;

        bool removeScript(std::shared_ptr<base::UUID> &uuid) override;

        void onUpdate() override;

        void onPhysics() override;

        void checkScript() override;
    };
}

#endif //DEMONENGINE_ASYNCSCRIPTPIPLINE_HPP
