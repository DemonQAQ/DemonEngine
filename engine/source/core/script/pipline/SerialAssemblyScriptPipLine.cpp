//
// Created by Demon on 2024/5/8.
//

#include "SerialAssemblyScriptPipLine.hpp"

namespace script
{
    SerialAssemblyScriptPipLine::SerialAssemblyScriptPipLine()
    {

    }

    bool SerialAssemblyScriptPipLine::submitScript(std::shared_ptr<IScriptEntity> &scriptEntity)
    {
        pendingAdditions.push_back(scriptEntity);
        return true;
    }

    bool SerialAssemblyScriptPipLine::removeScript(std::shared_ptr<base::UUID> &uuid)
    {
        pendingRemovals.push_back(*uuid);
        return true;
    }

    void SerialAssemblyScriptPipLine::onUpdate()
    {
        checkScript();
        for (auto &[uuid, script]: scripts)
        {
            if (script->hasMethod(onUpdateMethodName))
            {
                script->runMethod(onUpdateMethodName);
            }
        }
    }

    void SerialAssemblyScriptPipLine::onPhysics()
    {
        checkScript();
        for (auto &[uuid, script]: scripts)
        {
            if (script->hasMethod(onPhysicsMethodName))
            {
                script->runMethod(onPhysicsMethodName);
            }
        }
    }

    void SerialAssemblyScriptPipLine::checkScript()
    {
        for (const auto &uuid: pendingRemovals)
        {
            scripts.erase(uuid);
        }
        pendingRemovals.clear();

        for (const auto &script: pendingAdditions)
        {
            scripts[*(script->getUUID())] = script;
        }
        pendingAdditions.clear();
    }

    SerialAssemblyScriptPipLine::~SerialAssemblyScriptPipLine()
    {
        scripts.clear();
        pendingAdditions.clear();
    }
}