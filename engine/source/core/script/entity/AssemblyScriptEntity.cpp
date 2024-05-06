//
// Created by Demon on 2024/4/26.
//

#include "AssemblyScriptEntity.hpp"
#include "core/script/ScriptMethodType.hpp"
#include "mono/metadata/debug-helpers.h"
#include "mono/metadata/object.h"

script::AssemblyScriptEntity::AssemblyScriptEntity(const std::shared_ptr<base::UUID> &existingUuid, const std::string& name,
                                                   MonoAssembly *script, MonoImage *scriptImage, int priority_) :
        IScriptEntity(existingUuid,name),
        scriptAssembly(script), scriptImage(scriptImage), priority(priority_)
{

}

bool script::AssemblyScriptEntity::hasMethod(std::string &methodName)
{
    if (methodCache.find(methodName) != methodCache.end()) return true;

    MonoMethodDesc *desc = mono_method_desc_new(methodName.c_str(), true);
    MonoMethod *method = mono_method_desc_search_in_image(desc, scriptImage);
    mono_method_desc_free(desc);
    return method != nullptr;
}

bool script::AssemblyScriptEntity::runMethod(std::string &methodName)
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

void script::AssemblyScriptEntity::unload()
{
    if (scriptAssembly)
    {
        mono_assembly_close(scriptAssembly);
        scriptAssembly = nullptr;
        scriptImage = nullptr;
    }
}

void script::AssemblyScriptEntity::init()
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

bool script::AssemblyScriptEntity::operator<(const AssemblyScriptEntity &other) const
{
    return priority < other.priority;
}

bool script::AssemblyScriptEntity::operator>(const AssemblyScriptEntity &other) const
{
    return priority > other.priority;
}

bool script::AssemblyScriptEntity::operator==(const AssemblyScriptEntity &other) const
{
    return priority == other.priority;
}
