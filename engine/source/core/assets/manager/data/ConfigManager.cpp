//
// Created by Demon on 2024/3/10.
//

#include "ConfigManager.hpp"
#include "core/io/FileType.hpp"
#include "core/io/config/YamlConfiguration.hpp"
#include "core/io/config/JsonConfiguration.hpp"
#include "core/io/config/XmlConfiguration.hpp"

using namespace assets;

std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<io::IFile>> ConfigManager::loadedConfig;

/**
 *
 * @params[0] const std::shared_ptr<base::UUID> &existingUuid   文件的uuid
 * @params[1] std::string                                       文件路径
 * @parmas[2] io::FileType                                      文件类型
 * */
bool assets::ConfigManager::loadData(const std::vector<std::any> &params, bool isAssets)
{
    if (params.size() < 3)
    {
        std::cerr << "Invalid parameters for loadData on ConfigManager." << std::endl;
        return false;
    }

    std::shared_ptr<base::UUID> existingUuid;
    std::string path;
    io::FileType fileType;

    try
    {
        existingUuid = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
        auto it = loadedConfig.find(existingUuid);
        if (it != loadedConfig.end())
        {
            return true;
        }
        path = std::any_cast<std::string>(params[1]);
        fileType = std::any_cast<io::FileType>(params[2]);
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error extracting parameters: " << e.what() << std::endl;
        return false;
    }

    std::shared_ptr<io::IFile> configFile;
    switch (fileType)
    {
        case io::FileType::YAML:
            configFile = std::make_shared<io::YamlConfiguration>(existingUuid, path);
            break;
        case io::FileType::JSON:
            configFile = std::make_shared<io::JsonConfiguration>(existingUuid, path);
            break;
        case io::FileType::XML:
            configFile = std::make_shared<io::XmlConfiguration>(existingUuid, path);
            break;
        default:
            configFile = nullptr;
            break;
    }

    if (configFile)
    {
        loadedConfig[existingUuid] = configFile;
        return true;
    }
    return false;
}

void ConfigManager::unloadData(const std::vector<std::any> &params, bool isAssets)
{
    if (params.empty() || params[0].type() != typeid(base::UUID))return;

    auto uuid_ptr = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
    auto it = loadedConfig.find(uuid_ptr);
    if (it != loadedConfig.end())
    {
        loadedConfig.erase(it);
    }
}

bool ConfigManager::isDataLoaded(const std::vector<std::any> &params) const
{
    if (params.empty() || params[0].type() != typeid(base::UUID))return false;

    auto uuid_ptr = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
    return loadedConfig.find(uuid_ptr) != loadedConfig.end();
}

void ConfigManager::updateData(const std::vector<std::any> &params)
{

}

std::optional<std::shared_ptr<io::IFile>> ConfigManager::getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto it = loadedConfig.find(uuid_ptr);
    if (it != loadedConfig.end()) return {it->second};
    else return {};
}

void ConfigManager::onStart()
{

}

void ConfigManager::onStop()
{
    std::cerr << "ConfigManager onStop start" << std::endl;
    for (const auto& file: loadedConfig)file.second->save();
    std::cerr << "ConfigManager onStop end" << std::endl;
}
