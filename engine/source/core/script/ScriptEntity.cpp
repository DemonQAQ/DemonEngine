//
// Created by Demon on 2024/4/26.
//

#include "ScriptEntity.hpp"
#include "ScriptMethodType.hpp"
#include "mono/metadata/debug-helpers.h"
#include "mono/metadata/object.h"

script::ScriptEntity::ScriptEntity(const std::shared_ptr<base::UUID> &existingUuid, std::string name,
                                   MonoAssembly *script, int priority_) : base::Object(existingUuid),
                                                                          name(std::move(name)),
                                                                          scriptAssembly(script), priority(priority_)
{
    if (scriptAssembly)
    {
        scriptImage = mono_assembly_get_image(scriptAssembly);
        init();
    }
}

bool script::ScriptEntity::hasMethod(std::string &methodName)
{
    if (methodCache.find(methodName) != methodCache.end()) return true;

    MonoMethodDesc *desc = mono_method_desc_new(methodName.c_str(), true);
    MonoMethod *method = mono_method_desc_search_in_image(desc, scriptImage);
    mono_method_desc_free(desc);
    return method != nullptr;
}

bool script::ScriptEntity::runMethod(std::string &methodName)
{
    MonoMethod *method = nullptr;
    auto it = methodCache.find(methodName);
    if (it != methodCache.end())
    {
        method = it->second;
    }
    else
    {
        MonoMethodDesc *desc = mono_method_desc_new(methodName.c_str(), true);
        method = mono_method_desc_search_in_image(desc, scriptImage);
        mono_method_desc_free(desc);
        if (method)
        {
            methodCache[methodName] = method;
        }
    }

    if (!method)
    {
        std::cerr << "Method " << methodName << " not found." << std::endl;
        return false;
    }

    MonoObject *exception = nullptr;
    mono_runtime_invoke(method, nullptr, nullptr, &exception);
    if (exception)
    {
        mono_print_unhandled_exception(exception);
        return false;
    }
    return true;
}

void script::ScriptEntity::unload()
{
    if (scriptAssembly)
    {
        mono_assembly_close(scriptAssembly);
        scriptAssembly = nullptr;
        scriptImage = nullptr;
    }
}

void script::ScriptEntity::init()
{
    for (int i = ScriptMethodType::ON_UPDATE; i <= ScriptMethodType::ON_PHYSICS; i++)
    {
        std::string methodName = toString(static_cast<ScriptMethodType>(i));
        MonoMethodDesc *desc = mono_method_desc_new(methodName.c_str(), true);
        MonoMethod *method = mono_method_desc_search_in_image(desc, scriptImage);
        mono_method_desc_free(desc);
        if (method)
        {
            methodCache[methodName] = method;
        }
    }
}

void script::ScriptEntity::setName(const std::string &name_)
{
    this->name = name_;
}

std::string script::ScriptEntity::getName() const
{
    return name;
}

int script::ScriptEntity::getPriority() const
{
    return priority;
}

void script::ScriptEntity::setPriority(int newPriority_)
{
    this->priority = newPriority_;
}

bool script::ScriptEntity::operator<(const ScriptEntity &other) const
{
    return priority < other.priority;
}

bool script::ScriptEntity::operator>(const ScriptEntity &other) const
{
    return priority > other.priority;
}

bool script::ScriptEntity::operator==(const ScriptEntity &other) const
{
    return priority == other.priority;
}
