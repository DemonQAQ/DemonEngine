//
// Created by Demon on 2024/4/26.
//

#include <mono/metadata/appdomain.h>
#include "ScriptManager.hpp"
#include <mono/metadata/assembly.h>
#include <core/base/lib/BoolDefine.hpp>

std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<script::ScriptEntity>> assets::ScriptManager::loadScripts;

/**
 * @params[0] std::shared_ptr<base::UUID> &existingUuid         脚本实体uuid
 * @params[1] std::string path                                  脚本路径
 * @params[2] std::string name                                  脚本名称
 * */
bool assets::ScriptManager::loadData(const std::vector<std::any> &params)
{
    if (params.size() != 2)
    {
        std::cerr << "ScriptManager::loadData - Expected parameter" << std::endl;
        return false;
    }

    try
    {
        std::shared_ptr<base::UUID> existingUuid = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
        if (isDataLoaded({existingUuid}))return true;
        std::string scriptPath = std::any_cast<std::string>(params[1]);
        std::string name = std::any_cast<std::string>(params[2]);
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
        MonoAssembly *assembly = mono_assembly_load_from(image, scriptPath.c_str(), &status);
        mono_image_close(image);

        if (assembly)
        {
            auto scriptEntity = std::make_shared<script::ScriptEntity>(existingUuid, name, assembly);
            loadScripts[existingUuid] = scriptEntity;
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
    char scriptDomainName[] = "ScriptDomain";
    scriptDomain = mono_domain_create_appdomain(scriptDomainName, NULL);
    mono_domain_set(scriptDomain, false);
}

assets::ScriptManager::~ScriptManager()
{
    mono_domain_unload(scriptDomain);
}

std::optional<std::shared_ptr<script::ScriptEntity>>
assets::ScriptManager::getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto it = loadScripts.find(uuid_ptr);
    if (it != loadScripts.end())return it->second;
    else return std::nullopt;
}
