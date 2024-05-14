//
// Created by Demon on 2024/5/12.
//

#include <core/assets/manager/data/SkyBoxManager.hpp>
#include "SkyBoxLoader.hpp"
#include "ConfigLoader.hpp"
#include "core/io/FileSystem.hpp"
#include "core/assets/manager/AssetsDataMainManager.hpp"
#include "core/assets/AssetType.hpp"
#include <core/assets/manager/loader/TextureLoader.hpp>

std::shared_ptr<base::StaticSkybox>
assets::SkyBoxLoader::loadStaticSkybox(const std::string &name, const std::string &metaPath, const std::string &texturePath,
                                       bool isAssets, glm::vec3 emissiveColor, const std::shared_ptr<base::Shader> &shader)
{
    std::string dirPath = FileSystem::combinePaths(isAssets ? ASSETS_ROOT_PATH : RESOURCE_PACK_ROOT_PATH, metaPath);
    std::string metadataPath = dirPath.append( "/" + name + ".meta");

    bool init;
    auto metaYml = ConfigLoader::loadYml(metadataPath, true, true);
    if (!metaYml)return nullptr;
    init = metaYml->isEmpty();

    auto skyBoxManagerOpt = AssetsDataMainManager::getManager(AssetType::SKY_BOX);
    if (!skyBoxManagerOpt.has_value()) return nullptr;

    auto skyBoxManager = std::dynamic_pointer_cast<SkyBoxManager>(skyBoxManagerOpt.value());
    if (!skyBoxManager) return nullptr;

    auto skyBoxTexture = assets::TextureLoader::loadTexture(texturePath, isAssets, true);
    std::shared_ptr<base::Shader> normalShader = nullptr;
    if (!shader)
        normalShader = assets::ShaderLoader::loadShader("staticSkyBoxShader", "/shader/skybox",
                                                        "/shader/skybox/staticSkyBoxNormalShader.vsh",
                                                        "/shader/skybox/staticSkyBoxNormalShader.fsh", true);

    std::string uuidStr = utils::uuidUtil::getReappearUUID(name + metaPath + texturePath);
    const std::shared_ptr<base::UUID> existingUuid = base::UUIDManager::getUUID(uuidStr, true);

    if (skyBoxManager->loadData(
            {existingUuid, init, name, metaYml, base::SkyBoxType::STATIC, skyBoxTexture, emissiveColor, shader ? shader->getUUID() : normalShader->getUUID()}))
    {
        auto skyBoxOpt = skyBoxManager->getResourceByUuid(existingUuid);
        if (!skyBoxOpt.has_value()) return nullptr;

        auto skybox = std::dynamic_pointer_cast<base::StaticSkybox>(skyBoxOpt.value());
        return skybox;
    }
    else return nullptr;
}
