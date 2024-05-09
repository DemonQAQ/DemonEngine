//
// Created by Demon on 2024/4/26.
//

#include "AsyncAssemblyScriptPipLine.hpp"
#include "core/script/ScriptMethodType.hpp"

bool script::AsyncAssemblyScriptPipLine::submitScript(std::shared_ptr<IScriptEntity> &scriptEntity)
{
    pendingAdditions.push_back(scriptEntity);
    return true;
}

bool script::AsyncAssemblyScriptPipLine::removeScript(std::shared_ptr<base::UUID> &uuid)
{
    pendingRemovals.push_back(*uuid);
    return true;
}

void script::AsyncAssemblyScriptPipLine::onUpdate()
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

void script::AsyncAssemblyScriptPipLine::onPhysics()
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

void script::AsyncAssemblyScriptPipLine::checkScript()
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

script::AsyncAssemblyScriptPipLine::AsyncAssemblyScriptPipLine()
{
    thread = std::make_shared<MonoThread>("AsyncAssemblyScriptDomain", "AsyncAssemblyScriptThread");
    thread->start();
}

script::AsyncAssemblyScriptPipLine::~AsyncAssemblyScriptPipLine()
{
    scripts.clear();
    pendingAdditions.clear();
    thread->stop();
}