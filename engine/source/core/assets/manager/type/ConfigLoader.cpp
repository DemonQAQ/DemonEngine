//
// Created by Demon on 2024/4/13.
//

#include <core/base/utils/UUIDUtil.hpp>
#include <core/base/common/manager/UUIDManager.hpp>
#include <core/assets/manager/AssetsDataMainManager.hpp>
#include <core/assets/manager/data/ConfigManager.hpp>
#include "ConfigLoader.hpp"

std::shared_ptr<io::YamlConfiguration> assets::ConfigLoader::loadYml(const std::string &path, bool isYml)
{
    io::FileType fileType;
    if (isYml) fileType = io::FileType::YAML;
    else
    {
        fileType = getFileType(path);
        if (fileType != io::FileType::YAML)return nullptr;
    }

    std::string uuidStr = utils::uuidUtil::getReappearUUID(path);
    const std::shared_ptr<base::UUID> existingUuid = base::UUIDManager::getUUID(uuidStr, true);

    auto configManagerOpt = AssetsDataMainManager::getManager(AssetType::CONFIG);
    if (!configManagerOpt.has_value()) return nullptr;

    auto configManager = std::dynamic_pointer_cast<ConfigManager>(configManagerOpt.value());
    if (!configManager) return nullptr;

    if (configManager->loadData({existingUuid, path, fileType}))
    {
        auto yamlOpt = configManager->getResourceByUuid(existingUuid);
        if (!yamlOpt.has_value()) return nullptr;

        auto yaml = std::dynamic_pointer_cast<io::YamlConfiguration>(yamlOpt.value());
        return yaml;
    }
    else return nullptr;
}

std::shared_ptr<io::XmlConfiguration> assets::ConfigLoader::loadXml(const std::string &path, bool isXml)
{
    io::FileType fileType;
    if (isXml) fileType = io::FileType::XML;
    else
    {
        fileType = getFileType(path);
        if (fileType != io::FileType::XML)return nullptr;
    }

    std::string uuidStr = utils::uuidUtil::getReappearUUID(path);
    const std::shared_ptr<base::UUID> existingUuid = base::UUIDManager::getUUID(uuidStr, true);

    auto configManagerOpt = AssetsDataMainManager::getManager(AssetType::CONFIG);
    if (!configManagerOpt.has_value()) return nullptr;

    auto configManager = std::dynamic_pointer_cast<ConfigManager>(configManagerOpt.value());
    if (!configManager) return nullptr;

    if (configManager->loadData({existingUuid, path, fileType}))
    {
        auto xmlOpt = configManager->getResourceByUuid(existingUuid);
        if (!xmlOpt.has_value()) return nullptr;

        auto xml = std::dynamic_pointer_cast<io::XmlConfiguration>(xmlOpt.value());
        return xml;
    }
    else return nullptr;
}

std::shared_ptr<io::JsonConfiguration> assets::ConfigLoader::loadJson(const std::string &path, bool isJson)
{
    io::FileType fileType;
    if (isJson) fileType = io::FileType::JSON;
    else
    {
        fileType = getFileType(path);
        if (fileType != io::FileType::JSON)return nullptr;
    }

    std::string uuidStr = utils::uuidUtil::getReappearUUID(path);
    const std::shared_ptr<base::UUID> existingUuid = base::UUIDManager::getUUID(uuidStr, true);

    auto configManagerOpt = AssetsDataMainManager::getManager(AssetType::CONFIG);
    if (!configManagerOpt.has_value()) return nullptr;

    auto configManager = std::dynamic_pointer_cast<ConfigManager>(configManagerOpt.value());
    if (!configManager) return nullptr;

    if (configManager->loadData({existingUuid, path, fileType}))
    {
        auto jsonOpt = configManager->getResourceByUuid(existingUuid);
        if (!jsonOpt.has_value()) return nullptr;

        auto json = std::dynamic_pointer_cast<io::JsonConfiguration>(jsonOpt.value());
        return json;
    }
    else return nullptr;
}

io::FileType assets::ConfigLoader::getFileType(const std::string &path)
{
    auto dotPos = path.rfind('.');
    if (dotPos == std::string::npos)return io::FileType::NORMAL;

    std::string extension = path.substr(dotPos);
    return io::fromStringToFileType(extension);
}
