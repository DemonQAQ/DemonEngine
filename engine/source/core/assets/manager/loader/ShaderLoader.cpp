//
// Created by Demon on 2024/4/13.
//

#include <core/io/FileSystem.hpp>
#include <core/assets/manager/AssetsDataMainManager.hpp>
#include <core/assets/manager/data/ShaderManager.hpp>
#include <core/base/utils/UUIDUtil.hpp>
#include <core/base/common/manager/UUIDManager.hpp>
#include "ShaderLoader.hpp"
#include "ConfigLoader.hpp"

std::shared_ptr<base::Shader>
assets::ShaderLoader::loadShader(const std::string &name, const std::string &metaPath, const std::string &vertexPath,
                                 const std::string &fragmentPath, bool isAssets)
{
    std::string dirPath = FileSystem::combinePaths(isAssets ? ASSETS_ROOT_PATH : RESOURCE_PACK_ROOT_PATH, metaPath);
    std::string metadataPath = dirPath.append(name + ".meta");

    bool init;
    auto metaYml = ConfigLoader::loadYml(metadataPath, true, true);
    if (!metaYml)return nullptr;
    init = metaYml->isEmpty();

    auto shaderManagerOpt = AssetsDataMainManager::getManager(AssetType::SHADER);
    if (!shaderManagerOpt.has_value()) return nullptr;

    auto shaderManager = std::dynamic_pointer_cast<ShaderManager>(shaderManagerOpt.value());
    if (!shaderManager) return nullptr;

    std::string uuidStr = utils::uuidUtil::getReappearUUID(name + vertexPath + fragmentPath);
    const std::shared_ptr<base::UUID> existingUuid = base::UUIDManager::getUUID(uuidStr, true);

    if (shaderManager->loadData({existingUuid, init, name, vertexPath, fragmentPath, metaYml}, isAssets))
    {
        auto shaderOpt = shaderManager->getResourceByUuid(existingUuid);
        if (!shaderOpt.has_value()) return nullptr;

        auto shader = std::dynamic_pointer_cast<base::Shader>(shaderOpt.value());
        return shader;
    }
    else return nullptr;
}
