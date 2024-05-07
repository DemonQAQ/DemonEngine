//
// Created by Demon on 2024/5/8.
//

#ifndef DEMONENGINE_SERIALASSEMBLYSCRIPTPIPLINE_HPP
#define DEMONENGINE_SERIALASSEMBLYSCRIPTPIPLINE_HPP

#include "core/base/interface/Interface.hpp"
#include "core/script/interface/IScriptPipline.hpp"

namespace script
{
    class SerialAssemblyScriptPipLine : implements IScriptPipLine
    {
    private:
        std::unordered_map<base::UUID, std::shared_ptr<IScriptEntity>> scripts;
        std::vector<std::shared_ptr<IScriptEntity>> pendingAdditions;
        std::vector<base::UUID> pendingRemovals;
    public:
        SerialAssemblyScriptPipLine();

        bool submitScript(std::shared_ptr<IScriptEntity> &scriptEntity) override;

        bool removeScript(std::shared_ptr<base::UUID> &uuid) override;

        void onUpdate() override;

        void onPhysics() override;

    private:
        void checkScript() override;
    };

}

#endif //DEMONENGINE_SERIALASSEMBLYSCRIPTPIPLINE_HPP
