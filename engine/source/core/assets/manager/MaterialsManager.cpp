//
// Created by Demon on 2024/3/10.
//
#include <core/io/FileSystem.hpp>
#include "MaterialsManager.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "core/base/render/Material.hpp"
#include "TextureManager.hpp"
#include "AssetsMainManager.hpp"
#include "ConfigManager.hpp"

using namespace assets;

std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Material>> MaterialsManager::loadedMaterial;

MaterialsManager::MaterialsManager()
{
}

/**
 * 模式1
 * @params[0] std::string 资源文件架路径下.mat文件路径
 *
 * 模式2
 * @params[0] aiMaterial assimp的材质数据
 * @params[1] std::string 模型资源相对资源文件架的路径
 * */
std::optional<std::shared_ptr<base::UUID>> MaterialsManager::loadResource(const std::vector<std::any> &params)
{
    if (params.empty()) return std::nullopt;

    std::shared_ptr<base::Material> material;

    if (params[0].type() == typeid(const aiMaterial *))
    {
        std::string sourcePath;
        std::string metadataPath;
        const aiMaterial *aiMat;

        try
        {
            sourcePath = std::any_cast<std::string>(params[0]);
            sourcePath = FileSystem::combinePaths(SOURCE_ROOT_PATH, sourcePath);
            metadataPath = sourcePath + ".mat.meta";
            aiMat = std::any_cast<const aiMaterial *>(params[0]);
        } catch (
                const std::bad_any_cast &e
        )
        {
            std::cerr << "Error extracting parameters: " << e.what() << std::endl;
            return std::nullopt;
        }

        material = loadMaterialFromAssimp(aiMat, metadataPath);
    }
    else if (params[0].type() == typeid(io::YamlConfiguration))
    {
        const io::YamlConfiguration &yamlConfig = std::any_cast<io::YamlConfiguration>(params[0]);
        material = loadMaterialFromJson(yamlConfig);
    }
    else return std::nullopt;

    if (material)
    {
        auto uuid = material->getUUID();
        loadedMaterial[uuid] = material;
        return uuid;
    }
    else return std::nullopt;
}

std::shared_ptr<base::Material>
MaterialsManager::loadMaterialFromAssimp(const aiMaterial *aiMat, const std::string &metadataPath)
{
    auto configManagerOpt = AssetsMainManager::getManager(AssetType::CONFIG);
    if (!configManagerOpt.has_value()) return nullptr;
    auto configManager = std::dynamic_pointer_cast<ConfigManager>(configManagerOpt.value());
    if (!configManager) return nullptr;

    auto metaUuid = configManager->loadResource({metadataPath});
    if (!metaUuid.has_value())
    {
        std::cerr << "Failed to load metadata from: " << metadataPath << std::endl;
        return nullptr;
    }

    auto metaFileOpt = configManager->getResourceByUuid(metaUuid.value());
    if (!metaFileOpt.has_value())
    {
        std::cerr << "Failed to find metadata file with UUID: " << metaUuid.value()->toString() << std::endl;
        return nullptr;
    }

    auto metaFile = std::dynamic_pointer_cast<io::YamlConfiguration>(metaFileOpt.value());
    if (!metaFile)
    {
        std::cerr << "Metadata file is not a YAML configuration." << std::endl;
        return nullptr;
    }

    std::string uuidStr = metaFile->getString("uuid");
    bool init = uuidStr.empty();
    if (init) uuidStr = utils::uuidUtil::getUUID(metadataPath);
    auto materialUuid = UUIDManager::getUUID(uuidStr);
    auto it = loadedMaterial.find(materialUuid);
    if (it != loadedMaterial.end())
    {
        return it->second;
    }

    glm::vec3 diffuse(0.8f), specular(1.0f), ambient(0.2f), emissive(0.0f);
    float shininess = 32.0f, opacity = 1.0f, roughness = 0.5f, metallic = 0.0f, reflectivity = 0.5f;

    aiColor3D aiDiffuse(0.f, 0.f, 0.f);
    if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuse) == AI_SUCCESS)
    {
        diffuse = glm::vec3(aiDiffuse.r, aiDiffuse.g, aiDiffuse.b);
    }

    aiColor3D aiSpecular(0.f, 0.f, 0.f);
    if (aiMat->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecular) == AI_SUCCESS)
    {
        specular = glm::vec3(aiSpecular.r, aiSpecular.g, aiSpecular.b);
    }

    aiColor3D aiAmbient(0.f, 0.f, 0.f);
    if (aiMat->Get(AI_MATKEY_COLOR_AMBIENT, aiAmbient) == AI_SUCCESS)
    {
        ambient = glm::vec3(aiAmbient.r, aiAmbient.g, aiAmbient.b);
    }

    aiColor3D aiEmissive(0.f, 0.f, 0.f);
    if (aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, aiEmissive) == AI_SUCCESS)
    {
        emissive = glm::vec3(aiEmissive.r, aiEmissive.g, aiEmissive.b);
    }

    auto material = std::make_shared<base::Material>(materialUuid, init, metaFile, generateUniqueMaterialName(aiMat),
                                                     diffuse, specular, ambient, emissive,
                                                     shininess, opacity, roughness, metallic, reflectivity);
    return material;
}

std::shared_ptr<base::Material> MaterialsManager::loadMaterialFromJson(const io::YamlConfiguration &yamlConfig)
{
    return std::shared_ptr<base::Material>();
}


void MaterialsManager::unloadResource(const std::vector<std::any> &params)
{

}

bool MaterialsManager::isResourceLoaded(const std::vector<std::any> &params) const
{
    return false;
}

void MaterialsManager::updateResource(const std::vector<std::any> &params)
{

}

std::string MaterialsManager::generateUniqueMaterialName(const aiMaterial *aiMat)
{
    aiString name;
    aiMat->Get(AI_MATKEY_NAME, name);
    return name.C_Str();
}

std::optional<std::shared_ptr<base::Material>>
MaterialsManager::getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto it = loadedMaterial.find(uuid_ptr);
    if (it != loadedMaterial.end())return it->second;
    else return std::nullopt;
}

void MaterialsManager::updateMaterialRelation(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto material = getResourceByUuid(uuid_ptr);
    if (!material.has_value())return;

    auto metadata = material.value()->getMetadata();
    auto materialsUUIDs = std::any_cast<std::vector<std::shared_ptr<UUID>>>(
            metadata->getValue("textures"));
    auto textureManagerOpt = AssetsMainManager::getManager(AssetType::TEXTURE);
    if (!textureManagerOpt.has_value()) return;
    auto textureManager = std::dynamic_pointer_cast<TextureManager>(textureManagerOpt.value());
    if (!textureManager) return;

    for (const auto &uuid: materialsUUIDs)
    {
        auto texture = textureManager->getResourceByUuid(uuid);
        if (!texture.has_value())continue;
        material.value()->addTexture(texture.value());
    }
}

