//
// Created by Demon on 2024/4/26.
//

#ifndef DEMONENGINE_ISCRIPTPIPLINE_HPP
#define DEMONENGINE_ISCRIPTPIPLINE_HPP

#include "core/base/interface/Interface.hpp"
#include "core/script/entity/AssemblyScriptEntity.hpp"
#include "core/script/ScriptMethodType.hpp"

namespace script
{
    interface IScriptPipLine
    {
    protected:
        std::string onUpdateMethodName = toString(ScriptMethodType::ON_UPDATE);
        std::string onPhysicsMethodName = toString(ScriptMethodType::ON_PHYSICS);
    public:
        virtual bool submitScript(std::shared_ptr<IScriptEntity> &scriptEntity) = 0;

        virtual bool removeScript(std::shared_ptr<base::UUID> &uuid) = 0;

        virtual void onUpdate() = 0;

        virtual void onPhysics() = 0;

        virtual void checkScript() = 0;
    };
}

#endif //DEMONENGINE_ISCRIPTPIPLINE_HPP
