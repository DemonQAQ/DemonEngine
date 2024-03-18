//
// Created by Demon on 2024/3/10.
//

#include "ConfigManager.hpp"
#include "core/io/FileType.hpp"
#include "core/io/config/YamlConfiguration.hpp"
#include "core/io/config/JsonConfiguration.hpp"
#include "core/io/config/XmlConfiguration.hpp"

using namespace assets;

std::unordered_map<base::UUID, std::shared_ptr<io::IConfigurable>> ConfigManager::loadedConfig;

std::optional<base::UUID> assets::ConfigManager::LoadResource(const std::vector<std::any> &params)
{
    if (params.empty() || params[0].type() != typeid(std::string))
        return std::nullopt;

    const std::string &path = std::any_cast<std::string>(params[0]);
    // Extract file extension
    auto dotPos = path.rfind('.');
    if (dotPos == std::string::npos)return std::nullopt;

    std::string extension = path.substr(dotPos);
    io::FileType fileType = io::fromStringToFileType(extension);

    std::shared_ptr<io::IConfigurable> configFile;

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
    base::UUID uuid = configFile->getUuid();
    loadedConfig[uuid] = configFile;

    return uuid;
}

void ConfigManager::UnloadResource(const std::vector<std::any> &params)
{
    if (params.empty() || params[0].type() != typeid(base::UUID))return;

    const base::UUID &uuid = std::any_cast<base::UUID>(params[0]);
    auto it = loadedConfig.find(uuid);
    if (it != loadedConfig.end())
    {
        loadedConfig.erase(it);
    }
}

bool ConfigManager::IsResourceLoaded(const std::vector<std::any> &params) const
{
    if (params.empty() || params[0].type() != typeid(base::UUID))return false;

    const base::UUID &uuid = std::any_cast<base::UUID>(params[0]);
    return loadedConfig.find(uuid) != loadedConfig.end();
}

void ConfigManager::UpdateResource(const std::vector<std::any> &params)
{

}

std::optional<std::shared_ptr<io::IConfigurable>> ConfigManager::GetResourceByUuid(const base::UUID &uuid)
{
    auto it = loadedConfig.find(uuid);
    if(it != loadedConfig.end())return {it->second};
    else return {};
}
