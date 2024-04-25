//
// Created by Demon on 2024/4/26.
//

#include <mono/metadata/appdomain.h>
#include "ScriptManager.hpp"

bool assets::ScriptManager::loadData(const std::vector<std::any> &params)
{
    if (params.empty() || params[0].type() != typeid(std::string))
    {
        std::cerr << "ScriptManager::loadData - Expected parameter: std::string (path to script file)" << std::endl;
        return false;
    }

    try
    {
        std::string scriptPath = std::any_cast<std::string>(params[0]);
        std::vector<char> scriptData = loadFile(scriptPath);
        if (scriptData.empty())
        {
            return false;
        }

        MonoImageOpenStatus status;
        MonoImage *image = mono_image_open_from_data_with_name(scriptData.data(), scriptData.size(), TRUE, &status,
                                                               FALSE, scriptPath.c_str());
        if (status != MONO_IMAGE_OK || image == nullptr)
        {
            std::cerr << "Failed to load script image: " << scriptPath << std::endl;
            return false;
        }
        MonoAssembly* assembly = mono_assembly_load_from(image, scriptPath.c_str(), &status);
        mono_image_close(image); // Close image after loading the assembly

        if (assembly)
        {
            scriptAssemblies.push_back(assembly);
            return true;
        }
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error casting parameter: " << e.what() << std::endl;
    }
    return false;
}

void assets::ScriptManager::unloadData(const std::vector<std::any> &params)
{

}

bool assets::ScriptManager::isDataLoaded(const std::vector<std::any> &params) const
{
    return false;
}

void assets::ScriptManager::updateData(const std::vector<std::any> &params)
{

}

assets::ScriptManager::ScriptManager()
{
    scriptDomain = mono_domain_create_appdomain("ScriptDomain", NULL);
    mono_domain_set(scriptDomain, false);
}

assets::ScriptManager::~ScriptManager()
{

    mono_domain_unload(scriptDomain);
}
