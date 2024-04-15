//
// Created by Demon on 2024/3/10.
//
#include <core/io/FileSystem.hpp>
#include "MaterialsManager.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "core/base/render/Material.hpp"
#include "TextureManager.hpp"
#include "core/assets/manager/AssetsDataMainManager.hpp"
#include "ConfigManager.hpp"

using namespace assets;

std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Material>> MaterialsManager::loadedMaterial;

MaterialsManager::MaterialsManager()
{
}

/**
 *
 * @params[0] const std::shared_ptr<base::UUID> &existingUuid   材质的uuid
 * @params[1] bool init                                         是否第一次创建材质
 * @params[2] std::shared_ptr<io::YamlConfiguration> &yml       材质的meta文件
 *
 * 以下为可选参数
 * @params[3] std::string name_                                 材质名称
 * @params[4] glm::vec3 &diffuse_                               漫反射颜色
 * @params[5] glm::vec3 &specular_                              镜面反射颜色
 * @params[6] glm::vec3 &ambient_                               环境光颜色
 * @params[7] glm::vec3 &emissive_                              自发光颜色
 * @params[8] float shininess_                                  光泽度
 * @params[9] float opacity_                                    不透明度
 * @params[10] float roughness_                                 粗糙度
 * @params[11] float metallic_                                  金属度
 * @params[12] float reflectivity_                              反射率
 * @params[13] std::map<TextureType, std::map<std::shared_ptr<base::UUID>, std::shared_ptr<Texture>>> &textures_ 材质使用的贴图
 * */
bool MaterialsManager::loadData(const std::vector<std::any> &params)
{
    if (params.size() < 3) return false;

    std::shared_ptr<base::UUID> existingUuid;
    bool init;
    std::shared_ptr<io::YamlConfiguration> yml;
    std::string name;
    glm::vec3 diffuse;                 // 漫反射颜色
    glm::vec3 specular;                // 镜面反射颜色
    glm::vec3 ambient;                 // 环境光颜色
    glm::vec3 emissive;                // 自发光颜色
    float shininess;                   // 光泽度
    float opacity;                     // 不透明度
    float roughness;                   // 粗糙度
    float metallic;                    // 金属度
    float reflectivity;

    std::shared_ptr<Material> material;
    if (params.size() == 3)
    {
        try
        {
            existingUuid = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
            auto it = loadedMaterial.find(existingUuid);
            if (it != loadedMaterial.end())
            {
                return true;
            }

            init = std::any_cast<bool>(params[1]);
            yml = std::any_cast<std::shared_ptr<io::YamlConfiguration>>(params[2]);

            material = std::make_shared<Material>(existingUuid, init, yml);
        } catch (const std::bad_any_cast &e)
        {
            std::cerr << "Error extracting parameters: " << e.what() << std::endl;
            return false;
        }
    }
    else
    {
        try
        {
            existingUuid = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
            auto it = loadedMaterial.find(existingUuid);
            if (it != loadedMaterial.end())
            {
                return true;
            }

            init = std::any_cast<bool>(params[1]);
            yml = std::any_cast<std::shared_ptr<io::YamlConfiguration>>(params[2]);
            name = std::any_cast<std::string>(params[3]);
            diffuse = std::any_cast<glm::vec3>(params[4]);
            specular = std::any_cast<glm::vec3>(params[5]);
            ambient = std::any_cast<glm::vec3>(params[6]);
            emissive = std::any_cast<glm::vec3>(params[7]);
            shininess = std::any_cast<float>(params[8]);
            opacity = std::any_cast<float>(params[9]);
            roughness = std::any_cast<float>(params[10]);
            metallic = std::any_cast<float>(params[11]);
            reflectivity = std::any_cast<float>(params[12]);

            if (params.size() == 14)
            {
                auto textures = std::any_cast<std::map<TextureType, std::map<std::shared_ptr<base::UUID>, std::shared_ptr<Texture>>>>(
                        params[13]);
                material = std::make_shared<Material>(existingUuid, init, yml, name, diffuse, specular, ambient,
                                                      emissive,
                                                      shininess, opacity, roughness, metallic, reflectivity, textures);
            }
            else
                material = std::make_shared<Material>(existingUuid, init, yml, name, diffuse, specular, ambient,
                                                      emissive,
                                                      shininess, opacity, roughness, metallic, reflectivity);

        } catch (const std::bad_any_cast &e)
        {
            std::cerr << "Error extracting parameters: " << e.what() << std::endl;
            return false;
        }
    }

    if (material)
    {
        loadedMaterial[existingUuid] = material;
        return true;
    }
    else return false;
}

void MaterialsManager::unloadData(const std::vector<std::any> &params)
{

}

/**
 *
 * @params[0] const std::shared_ptr<base::UUID> &existingUuid   材质的uuid
 * */
bool MaterialsManager::isDataLoaded(const std::vector<std::any> &params) const
{
    if (params.size() != 1) return false;

    std::shared_ptr<base::UUID> existingUuid;

    try
    {
        existingUuid = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
        auto it = loadedMaterial.find(existingUuid);
        if (it != loadedMaterial.end())
        {
            return true;
        }
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error extracting parameters: " << e.what() << std::endl;
        return false;
    }
}

void MaterialsManager::updateData(const std::vector<std::any> &params)
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
    auto textureManagerOpt = AssetsDataMainManager::getManager(AssetType::TEXTURE);
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

