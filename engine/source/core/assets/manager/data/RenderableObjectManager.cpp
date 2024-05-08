//
// Created by Demon on 2024/4/20.
//

#include "RenderableObjectManager.hpp"

using namespace assets;

std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::RenderableObject>> RenderableObjectManager::loadedRenderableObjects;

/**
 * @params[0] std::shared_ptr<base::UUID> &existingUuid         渲染实体uuid
 * @params[1] bool init                                         是否第一次创建
 * @params[2] std::string renderObjectName                      渲染实体名称
 * @params[3] std::vector<std::shared_ptr<Model>> &models       模型的组数据
 * @params[4] std::shared_ptr<io::YamlConfiguration> &yml       meta文件
 * @params[5] base::Transform &initialTransform                 变换数据
 * @params[6] std::shared_ptr<base::UUID> &shaderUUID           使用的shader的uuid
 * @params[7] std::shared_ptr<base::UUID> &materialUUID         使用的材质的uuid
 * */
bool RenderableObjectManager::loadData(const std::vector<std::any> &params)
{
    if (params.size() < 4)
    {
        std::cerr << "Invalid parameters for loadData on RenderableObjectManager." << std::endl;
        return false;
    }

    std::shared_ptr<base::UUID> existingUuid;
    bool init;
    std::string renderObjectName;
    std::vector<std::shared_ptr<base::Model>> models;
    std::shared_ptr<io::YamlConfiguration> yml;
    base::Transform initialTransform;
    std::shared_ptr<base::UUID> shaderUUID;
    std::shared_ptr<base::UUID> materialUUID;
    try
    {
        existingUuid = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
        auto it = loadedRenderableObjects.find(existingUuid);
        if (it != loadedRenderableObjects.end())
        {
            return true;
        }

        init = std::any_cast<bool>(params[1]);
        renderObjectName = std::any_cast<std::string>(params[2]);
        yml = std::any_cast<std::shared_ptr<io::YamlConfiguration>>(params[3]);

        models = std::any_cast<std::vector<std::shared_ptr<base::Model>>>(params[4]);
        initialTransform = (params.size() >= 6) ? std::any_cast<base::Transform>(params[5]) : base::Transform();
        shaderUUID = (params.size() >= 7) ? std::any_cast<std::shared_ptr<base::UUID>>(params[6]) : nullptr;
        materialUUID = (params.size() >= 8) ? std::any_cast<std::shared_ptr<base::UUID>>(params[7]) : nullptr;
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error extracting parameters: " << e.what() << std::endl;
        return false;
    }

    auto renderableObject = std::make_shared<base::RenderableObject>(existingUuid,init,renderObjectName,yml,models,initialTransform,shaderUUID,materialUUID);

    if (!renderableObject)
    {
        return false;
    }
    loadedRenderableObjects[existingUuid] = renderableObject;
    return true;
}

void RenderableObjectManager::unloadData(const std::vector<std::any> &params)
{

}

bool RenderableObjectManager::isDataLoaded(const std::vector<std::any> &params) const
{
    return false;
}

void assets::RenderableObjectManager::updateData(const std::vector<std::any> &params)
{

}

std::optional<std::shared_ptr<base::RenderableObject>> RenderableObjectManager::getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto it = loadedRenderableObjects.find(uuid_ptr);
    if (it != loadedRenderableObjects.end())return it->second;
    else return std::nullopt;
}

void RenderableObjectManager::onStart()
{

}

void RenderableObjectManager::onStop()
{
    std::cerr << "RenderableObjectManager onStop start" << std::endl;
    std::cerr << "RenderableObjectManager onStop end" << std::endl;

}
