//
// Created by Demon on 2024/4/26.
//

#include "SerialAssemblyScriptPipLine.hpp"
#include "core/script/ScriptMethodType.hpp"

bool script::SerialAssemblyScriptPipLine::submitScript(std::shared_ptr<IScriptEntity> &scriptEntity)
{
    pendingAdditions.push_back(scriptEntity);
    return true;
}

bool script::SerialAssemblyScriptPipLine::removeScript(std::shared_ptr<base::UUID> &uuid)
{
    pendingRemovals.push_back(*uuid);
    return true;
}

void script::SerialAssemblyScriptPipLine::onUpdate()
{
    checkScript();
    for (auto &[uuid, script]: scripts)
    {
        if (script->hasMethod(onUpdateMethodName))
        {
            script->runMethod(onPhysicsMethodName);
        }
    }
}

void script::SerialAssemblyScriptPipLine::onPhysics()
{
    checkScript();
    for (auto &[uuid, script]: scripts)
    {
        if (script->hasMethod(onUpdateMethodName))
        {
            script->runMethod(onPhysicsMethodName);
        }
    }
}

void script::SerialAssemblyScriptPipLine::checkScript()
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

script::SerialAssemblyScriptPipLine::SerialAssemblyScriptPipLine()
{
    thread = std::make_shared<MonoThread>("SerialScriptPipLineDomain");
    thread->start();
}
