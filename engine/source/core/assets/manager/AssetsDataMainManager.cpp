//
// Created by Demon on 2024/3/15.
//
#include "core/assets/manager/data/ModelManager.hpp"
#include "core/assets/manager/data/MaterialsManager.hpp"
#include "AssetsDataMainManager.hpp"
#include "core/assets/manager/data/ConfigManager.hpp"
#include "core/assets/manager/data/ShaderManager.hpp"
#include "core/assets/manager/data/MediaManager.hpp"
#include "core/assets/manager/data/TextureManager.hpp"


namespace assets
{
    std::unordered_map<AssetType, std::shared_ptr<IDataManager>> AssetsDataMainManager::MANAGERS;

    void AssetsDataMainManager::initializeManager()
    {
        MANAGERS.insert({AssetType::TEXTURE, std::make_shared<TextureManager>()});
        MANAGERS.insert({AssetType::MODEL, std::make_shared<ModelManager>()});
        MANAGERS.insert({AssetType::MATERIALS, std::make_shared<MaterialsManager>()});
        MANAGERS.insert({AssetType::CONFIG, std::make_shared<ConfigManager>()});
        MANAGERS.insert({AssetType::SHADER, std::make_shared<ShaderManager>()});
        MANAGERS.insert({AssetType::MEDIA, std::make_shared<MediaManager>()});
    }

    void AssetsDataMainManager::finalizeManager()
    {

    }

    std::string AssetsDataMainManager::getManagerClassName(AssetType assetType)
    {
        switch (assetType)
        {
            case AssetType::TEXTURE:
                return "TextureManager";
            case AssetType::MODEL:
                return "ModelManager";
            case AssetType::MATERIALS:
                return "MaterialsManager";
            case AssetType::CONFIG:
                return "ConfigManager";
            case AssetType::SHADER:
                return "ShaderManager";
            case AssetType::MEDIA:
                return "MediaManager";
            default:
                throw std::runtime_error("Unknown assetType");
        }
    }

    std::optional<std::shared_ptr<IDataManager>> AssetsDataMainManager::getManager(AssetType assetType)
    {
        auto it = MANAGERS.find(assetType);
        if (it != MANAGERS.end()) return it->second;
        else return std::nullopt;
    }

}

