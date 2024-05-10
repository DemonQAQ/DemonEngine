//
// Created by Demon on 2024/4/26.
//

#include <mono/metadata/appdomain.h>
#include "AssemblyScriptEntity.hpp"
#include "core/script/ScriptMethodType.hpp"
#include "mono/metadata/debug-helpers.h"
#include "mono/metadata/object.h"

script::AssemblyScriptEntity::AssemblyScriptEntity(const std::shared_ptr<base::UUID> &existingUuid,
                                                   const std::string &name,
                                                   MonoAssembly *script, MonoImage *scriptImage, int priority_) :
        IScriptEntity(existingUuid, name),
        scriptAssembly(script), scriptImage(scriptImage), priority(priority_)
{
    className = getFirstClassName(scriptImage);
    if (!className.empty())
    {
        MonoClass *classMono = mono_class_from_name(scriptImage, "", className.c_str());
        if (classMono)
        {
            instance = mono_object_new(mono_domain_get(), classMono);
            if (instance)
            {
                mono_runtime_object_init(instance);
            }
            else
            {
                std::cerr << "Failed to create an instance of the class: " << className << std::endl;
            }
        }
        else
        {
            std::cerr << "Failed to find the class: " << className << std::endl;
        }
    }
    else
    {
        std::cerr << "No class name found in the script image." << std::endl;
    }
}

bool script::AssemblyScriptEntity::hasMethod(std::string &methodName)
{
    if (cleanUp)return false;
    if (methodCache.find(methodName) != methodCache.end()) return true;

    MonoMethodDesc *desc = mono_method_desc_new((className + "::" + methodName).c_str(), true);
    MonoMethod *method = mono_method_desc_search_in_image(desc, scriptImage);
    mono_method_desc_free(desc);
    return method != nullptr;
}

bool script::AssemblyScriptEntity::runMethod(std::string &methodName)
{
    if (cleanUp)return false;
    MonoMethod *method = nullptr;
    auto it = methodCache.find(methodName);
    if (it != methodCache.end())
    {
        method = it->second;
    }
    else
    {
        MonoMethodDesc *desc = mono_method_desc_new((className + "::" + methodName).c_str(), true);
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
    mono_runtime_invoke(method, instance, nullptr, &exception);
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

std::string script::AssemblyScriptEntity::getFirstClassName(MonoImage *image)
{
    if (cleanUp)return "";
    const MonoTableInfo *table_info = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
    int rows = mono_table_info_get_rows(table_info);
    for (int i = 1; i < rows; i++)
    {
        uint32_t cols[MONO_TYPEDEF_SIZE];
        mono_metadata_decode_row(table_info, i, cols, MONO_TYPEDEF_SIZE);
        const char *name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
        return std::string(name);
    }
    return ""; // No class found
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

script::AssemblyScriptEntity::~AssemblyScriptEntity()
{
    cleanupResources();
}

void script::AssemblyScriptEntity::cleanupResources()
{
    if (instance)
    {
        auto handle = mono_gchandle_new(instance, false);
        mono_gchandle_free(handle);
        instance = nullptr;
    }

    for (auto &pair: methodCache)
    {
        pair.second = nullptr;
    }
    methodCache.clear();

    if (scriptImage)
    {
        mono_image_close(scriptImage);
        scriptImage = nullptr;
    }

    if (scriptAssembly)
    {
        scriptAssembly = nullptr;
    }
    cleanUp = true;
}
