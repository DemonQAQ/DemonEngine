//
// Created by Demon on 2024/5/13.
//

#include <core/io/FileSystem.hpp>
#include "TextureLoader.hpp"
#include "ConfigLoader.hpp"
#include "core/base/utils/UUIDUtil.hpp"
#include "core/base/common/manager/UUIDManager.hpp"
#include "core/assets/manager/AssetsDataMainManager.hpp"
#include "core/assets/AssetType.hpp"
#include "core/assets/manager/data/TextureManager.hpp"

namespace assets
{
    std::shared_ptr<base::Texture> TextureLoader::loadTexture(const std::string &path, bool isAssets)
    {
        base::TextureType textureType = extractTypeNameFromPath(path);
        std::string dirPath = FileSystem::combinePaths(isAssets ? ASSETS_ROOT_PATH : RESOURCE_PACK_ROOT_PATH, path);
        std::string metadataPath = dirPath + ".meta";
        auto metaYml = ConfigLoader::loadYml(metadataPath, true, true, isAssets);
        if (!metaYml)return nullptr;

        bool init = metaYml->isEmpty();
        std::shared_ptr<base::UUID> existingUuid;
        std::string uuidStr;

        if (init)uuidStr = utils::uuidUtil::getUUID();
        else uuidStr = metaYml->getString("uuid");

        existingUuid = base::UUIDManager::getUUID(uuidStr, false);

        auto texturesManagerOpt = AssetsDataMainManager::getManager(AssetType::TEXTURE);
        if (!texturesManagerOpt.has_value()) return nullptr;
        auto texturesManager = std::dynamic_pointer_cast<TextureManager>(texturesManagerOpt.value());
        if (!texturesManager) return nullptr;

        bool isLoad = texturesManager->isDataLoaded({existingUuid});

        if (isLoad || texturesManager->loadData({existingUuid, init, metaYml, textureType, dirPath}))
        {
            auto textureOpt = texturesManager->getResourceByUuid(existingUuid);
            if (!textureOpt.has_value()) return nullptr;

            auto texture = std::dynamic_pointer_cast<base::Texture>(textureOpt.value());
            return texture;
        }
        else return nullptr;
    }

    base::TextureType TextureLoader::extractTypeNameFromPath(const std::string &path)
    {
        size_t lastSlash = path.find_last_of("/\\");
        std::string filename = lastSlash != std::string::npos ? path.substr(lastSlash + 1) : path;
        size_t underscorePos = filename.find_last_of('_');
        if (underscorePos != std::string::npos && underscorePos + 1 < filename.size())
        {
            size_t dotPos = filename.find('.', underscorePos);
            if (dotPos != std::string::npos)
            {
                return base::fromStringToTextureType(filename.substr(underscorePos, dotPos - underscorePos));
            }
        }
        return base::TextureType::DIFFUSE;
    }
}