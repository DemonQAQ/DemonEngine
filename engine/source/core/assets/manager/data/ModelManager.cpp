//
// Created by Demon on 2024/3/9.
//
#include "ModelManager.hpp"
#include "MaterialsManager.hpp"
#include "assimp/Importer.hpp"
#include "ConfigManager.hpp"
#include <iostream>
#include <core/io/FileSystem.hpp>

using namespace assets;
using namespace base;

std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Model>> ModelManager::loadedModels;

/**
 * @params[0] std::shared_ptr<base::UUID> &existingUuid         模型uuid
 * @params[1] bool init                                         是否第一次创建模型
 * @params[2] std::string modelName                             模型名称
 * @params[3] const std::shared_ptr<Node> &root                 模型的组数据
 * @params[4] std::shared_ptr<io::YamlConfiguration> &yml       meta文件
 *
 * 以下为可选项，不传入默认使用nullptr
 * @params[5] const Transform &initialTransform                 模型的变换数据
 * @params[6] const std::shared_ptr<base::UUID> &shaderUUID     使用的shader的uuid
 * @params[7] const std::shared_ptr<base::UUID> &materialUUID   使用的材质的uuid
 * */
bool ModelManager::loadData(const std::vector<std::any> &params)
{
    if (params.size() < 5)
    {
        std::cerr << "Invalid parameters for loadData on ModelManager." << std::endl;
        return false;
    }

    std::shared_ptr<base::UUID> existingUuid;
    bool init;
    std::string modelName;
    std::shared_ptr<Node> root;
    std::shared_ptr<io::YamlConfiguration> yml;
    Transform initialTransform;
    std::shared_ptr<base::UUID> shaderUUID;
    std::shared_ptr<base::UUID> materialUUID;
    try
    {
        existingUuid = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
        auto it = loadedModels.find(existingUuid);
        if (it != loadedModels.end())
        {
            return true;
        }

        init = std::any_cast<bool>(params[1]);
        modelName = std::any_cast<std::string>(params[2]);
        root = std::any_cast<std::shared_ptr<Node>>(params[3]);
        yml = std::any_cast<std::shared_ptr<io::YamlConfiguration>>(params[4]);

        initialTransform = (params.size() >= 6) ? std::any_cast<Transform>(params[5]) : Transform();
        shaderUUID = (params.size() >= 7) ? std::any_cast<std::shared_ptr<base::UUID>>(params[6]) : nullptr;
        materialUUID = (params.size() >= 8) ? std::any_cast<std::shared_ptr<base::UUID>>(params[7]) : nullptr;
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error extracting parameters: " << e.what() << std::endl;
        return false;
    }

    auto model = std::make_shared<Model>(existingUuid, init, modelName, root, yml, initialTransform, shaderUUID,
                                         materialUUID);
    if (!model)
    {
        return false;
    }
    loadedModels[existingUuid] = model;
    return true;
}

void ModelManager::unloadData(const std::vector<std::any> &params)
{
    // Implementation of unloadData
}

bool ModelManager::isDataLoaded(const std::vector<std::any> &params) const
{
    // Implementation of isDataLoaded
    return false;
}

void ModelManager::updateData(const std::vector<std::any> &params)
{
    // Implementation of updateData
}

base::Transform ModelManager::convertAiMatrixToTransform(const aiMatrix4x4 &aiMatrix)
{
    glm::mat4 matrix;
    matrix[0][0] = aiMatrix.a1;
    matrix[1][0] = aiMatrix.a2;
    matrix[2][0] = aiMatrix.a3;
    matrix[3][0] = aiMatrix.a4;
    matrix[0][1] = aiMatrix.b1;
    matrix[1][1] = aiMatrix.b2;
    matrix[2][1] = aiMatrix.b3;
    matrix[3][1] = aiMatrix.b4;
    matrix[0][2] = aiMatrix.c1;
    matrix[1][2] = aiMatrix.c2;
    matrix[2][2] = aiMatrix.c3;
    matrix[3][2] = aiMatrix.c4;
    matrix[0][3] = aiMatrix.d1;
    matrix[1][3] = aiMatrix.d2;
    matrix[2][3] = aiMatrix.d3;
    matrix[3][3] = aiMatrix.d4;

    base::Transform transform;
    transform.fromMatrix(matrix);
    return transform;
}

std::optional<std::shared_ptr<base::Model>> ModelManager::getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto it = loadedModels.find(uuid_ptr);
    if (it != loadedModels.end())return it->second;
    else return std::nullopt;
}

void ModelManager::updateModelRelation(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto model = getResourceByUuid(uuid_ptr);
    if (!model.has_value())return;

    auto metadata = model.value()->getMetadata();
    auto materialsUUIDMap = std::any_cast<std::map<std::shared_ptr<UUID>, std::shared_ptr<UUID>>>(
            metadata->getValue("materials"));

    auto selfMaterialUuid = materialsUUIDMap[uuid_ptr];
    if (selfMaterialUuid) model.value()->bindMaterial(selfMaterialUuid);

    std::function<void(const std::shared_ptr<Node> &)> updateMeshMaterial;
    updateMeshMaterial = [&materialsUUIDMap, &updateMeshMaterial, &selfMaterialUuid](const std::shared_ptr<Node> &node)
    {
        for (const auto &mesh: node->meshes)
        {
            auto it = materialsUUIDMap.find(mesh->getUUID());
            if (it != materialsUUIDMap.end())mesh->bindMaterial(it->second);
            else mesh->bindMaterial(selfMaterialUuid);
        }
        for (const auto &child: node->children)
        {
            updateMeshMaterial(child);
        }
    };
    updateMeshMaterial(model.value()->getRootNode());
}
