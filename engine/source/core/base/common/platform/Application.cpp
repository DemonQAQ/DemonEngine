//
// Created by Demon on 2024/4/3.
//
#include "Application.hpp"

double base::Application::deltaTime = 0;

bool base::Application::submitScript(std::shared_ptr<script::IScriptEntity> scriptEntity, bool async)
{
    if (async)return asyncScriptPipLine->submitScript(scriptEntity);
    else return serialScriptPipLine->submitScript(scriptEntity);
}

void base::Application::removeScript(std::shared_ptr<UUID> &uuid, bool async)
{
    if (async) asyncScriptPipLine->removeScript(uuid);
    else serialScriptPipLine->removeScript(uuid);
}

std::shared_ptr<render::RenderManager> &base::Application::getRenderManager()
{
    return renderManager;
}

void base::Application::callEvent(const std::shared_ptr<event::IEvent> &event)
{
    eventBus->callEvent(event);
}

void base::Application::subscribe(const std::shared_ptr<event::IEventListener> &listener)
{
    eventBus->subscribe(listener);
    listenerList.push_back(listener);
}

double base::Application::getDeltaTime()
{
    return deltaTime;
}