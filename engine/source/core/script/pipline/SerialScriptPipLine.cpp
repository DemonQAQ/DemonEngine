//
// Created by Demon on 2024/4/26.
//

#include "SerialScriptPipLine.hpp"
#include "core/script/ScriptMethodType.hpp"

bool script::SerialScriptPipLine::submitScript(std::shared_ptr<ScriptEntity> &scriptEntity)
{
    pendingAdditions.push_back(scriptEntity);
    return true;
}

bool script::SerialScriptPipLine::removeScript(std::shared_ptr<base::UUID> &uuid)
{
    pendingRemovals.push_back(*uuid);
    return true;
}

void script::SerialScriptPipLine::onUpdate()
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

void script::SerialScriptPipLine::onPhysics()
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

void script::SerialScriptPipLine::checkScript()
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
