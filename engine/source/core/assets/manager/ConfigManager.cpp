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
 * @params[0] std::string 文件路径
 * */
std::optional<std::shared_ptr<base::UUID>> assets::ConfigManager::loadResource(const std::vector<std::any> &params)
{
    if (params.empty() || params[0].type() != typeid(std::string))
        return std::nullopt;

    const std::string &path = std::any_cast<std::string>(params[0]);
    // Extract file extension
    auto dotPos = path.rfind('.');
    if (dotPos == std::string::npos)return std::nullopt;

    std::string extension = path.substr(dotPos);
    io::FileType fileType = io::fromStringToFileType(extension);

    std::shared_ptr<io::IFile> configFile;

    switch (fileType)
    {
        case io::FileType::YAML:
            configFile = std::make_shared<io::YamlConfiguration>(path);
            break;
        case io::FileType::JSON:
            configFile = std::make_shared<io::JsonConfiguration>(path);
            break;
        case io::FileType::XML:
            configFile = std::make_shared<io::XmlConfiguration>(path);
            break;
        default:
            return std::nullopt;
    }

    configFile->load(path);
    auto uuid = configFile->getUUID();
    loadedConfig[uuid] = configFile;

    return uuid;
}

void ConfigManager::unloadResource(const std::vector<std::any> &params)
{
    if (params.empty() || params[0].type() != typeid(base::UUID))return;

    auto uuid_ptr = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
    auto it = loadedConfig.find(uuid_ptr);
    if (it != loadedConfig.end())
    {
        loadedConfig.erase(it);
    }
}

bool ConfigManager::isResourceLoaded(const std::vector<std::any> &params) const
{
    if (params.empty() || params[0].type() != typeid(base::UUID))return false;

    auto uuid_ptr = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
    return loadedConfig.find(uuid_ptr) != loadedConfig.end();
}

void ConfigManager::updateResource(const std::vector<std::any> &params)
{

}

std::optional<std::shared_ptr<io::IFile>> ConfigManager::getResourceByUuid(const std::shared_ptr<base::UUID>& uuid_ptr)
{
    auto it = loadedConfig.find(uuid_ptr);
    if (it != loadedConfig.end()) return {it->second};
    else return {};
}
