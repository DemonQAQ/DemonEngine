//
// Created by Demon on 2024/4/26.
//

#ifndef DEMONENGINE_SERIALSCRIPTPIPLINE_HPP
#define DEMONENGINE_SERIALSCRIPTPIPLINE_HPP

#include "core/base/interface/Interface.hpp"
#include "core/script/interface/IScriptPipline.hpp"

namespace script
{
    class SerialScriptPipLine : implements IScriptPipLine
    {
    private:
        std::unordered_map<base::UUID, std::shared_ptr<ScriptEntity>> scripts;
        std::vector<std::shared_ptr<ScriptEntity>> pendingAdditions;
        std::vector<base::UUID> pendingRemovals;
    public:
        bool submitScript(std::shared_ptr<ScriptEntity> &scriptEntity) override;

        bool removeScript(std::shared_ptr<base::UUID> &uuid) override;

        void onUpdate() override;

        void onPhysics() override;
    private:
        void checkScript() override;
    };
}

#endif //DEMONENGINE_SERIALSCRIPTPIPLINE_HPP
