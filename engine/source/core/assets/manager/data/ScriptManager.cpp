//
// Created by Demon on 2024/4/26.
//

#include "ScriptManager.hpp"
#include <mono/metadata/assembly.h>
#include <core/io/FileSystem.hpp>

std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<script::IScriptEntity>> assets::ScriptManager::loadScripts;

/**
 * @params[0] std::shared_ptr<base::UUID> &existingUuid         脚本实体uuid
 * @params[1] std::string path                                  脚本路径
 * @params[2] std::string name                                  脚本名称
 * */
bool assets::ScriptManager::loadData(const std::vector<std::any> &params)
{
    if (params.size() != 3)
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
        std::string fullScriptPath = FileSystem::combinePaths(SOURCE_ROOT_PATH,scriptPath);
        std::vector<char> scriptData = loadFile(fullScriptPath);
        if (scriptData.empty())
        {
            return false;
        }

        auto scriptEntity = thread->compileCSharpFile(existingUuid, fullScriptPath, name, scriptData);
        loadScripts[existingUuid] = scriptEntity;
        return true;
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
    thread = std::make_shared<script::MonoAssemblyCompileThread>("MonoAssemblyCompileThread");
    thread->start();
}

assets::ScriptManager::~ScriptManager()
{
    thread->stop();
}

std::optional<std::shared_ptr<script::IScriptEntity>>
assets::ScriptManager::getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto it = loadScripts.find(uuid_ptr);
    if (it != loadScripts.end())return it->second;
    else return std::nullopt;
}
