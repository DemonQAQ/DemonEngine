﻿//
// Created by Demon on 2024/3/10.
//
#include "MaterialsManager.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "core/base/render/Material.hpp"
#include "TextureManager.hpp"
#include "AssetsMainManager.hpp"

using namespace assets;

std::unordered_map<base::UUID, std::shared_ptr<base::Material>> MaterialsManager::loadedMaterial;

std::optional<base::UUID> MaterialsManager::LoadResource(const std::vector<std::any> &params)
{
    if (params.empty()) return std::nullopt;

    std::shared_ptr<base::Material> material;

    if (params[0].type() == typeid(const aiMaterial *))
    {
        const aiMaterial *aiMat = std::any_cast<const aiMaterial *>(params[0]);
        material = LoadMaterialFromAssimp(aiMat);
    } else if (params[0].type() == typeid(io::JsonConfiguration))
    {
        const io::JsonConfiguration &jsonConfig = std::any_cast<io::JsonConfiguration>(params[0]);
        material = LoadMaterialFromJson(jsonConfig);
    } else return std::nullopt;

    if (material)
    {
        base::UUID uuid = material->getUuid();
        loadedMaterial[uuid] = material;
        return uuid;
    } else return std::nullopt;
}

std::shared_ptr<base::Material> MaterialsManager::LoadMaterialFromAssimp(const aiMaterial *aiMat)
{
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

    //aiMat->Get(AI_MATKEY_SHININESS, shininess);
    //aiMat->Get(AI_MATKEY_OPACITY, opacity);
    // Roughness, Metallic, Reflectivity 不是Assimp直接支持的标准属性，可能需要自定义处理或映射

    auto textureManagerOpt = AssetsMainManager::getManager(AssetType::TEXTURE);
    if (!textureManagerOpt.has_value()) return nullptr;
    auto textureManager = std::dynamic_pointer_cast<TextureManager>(textureManagerOpt.value());
    if (!textureManager) return nullptr;

    std::map<base::TextureType, std::map<base::UUID, std::shared_ptr<base::Texture>>> textures;

    for (auto aiTexType: supportedAiTextureTypes)
    {
        aiString texPath;
        if (AI_SUCCESS == aiMat->GetTexture(aiTexType, 0, &texPath))
        {
            std::vector<std::any> textureParams = {std::string(texPath.C_Str()), aiTextureTypeToTextureType(aiTexType)};
            auto textureUuid = textureManager->LoadResource(textureParams);
            if (textureUuid.has_value())
            {
                auto textureOpt = textureManager->GetResourceByUuid(textureUuid.value());
                if (textureOpt.has_value())
                {
                    auto texture = textureOpt.value();
                    textures[aiTextureTypeToTextureType(aiTexType)][texture->getUuid()] = texture;
                }
            }
        }
    }

    auto material = std::make_shared<base::Material>(GenerateUniqueMaterialName(aiMat), diffuse, specular, ambient, emissive,
                                                     shininess, opacity, roughness, metallic, reflectivity, textures);
    return material;
}

std::shared_ptr<base::Material> MaterialsManager::LoadMaterialFromJson(const io::JsonConfiguration &jsonConfig)
{
    return std::shared_ptr<base::Material>();
}


void MaterialsManager::UnloadResource(const std::vector<std::any> &params)
{

}

bool MaterialsManager::IsResourceLoaded(const std::vector<std::any> &params) const
{
    return false;
}

void MaterialsManager::UpdateResource(const std::vector<std::any> &params)
{

}

std::string MaterialsManager::GenerateUniqueMaterialName(const aiMaterial *aiMat)
{
    aiString name;
    aiMat->Get(AI_MATKEY_NAME, name);
    return name.C_Str();
}

std::optional<std::shared_ptr<base::Material>> MaterialsManager::GetResourceByUuid(const base::UUID &uuid)
{
    return std::optional<std::shared_ptr<base::Material>>();
}

