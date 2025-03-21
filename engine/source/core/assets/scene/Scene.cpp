﻿//
// Created by Demon on 2024/3/27.
//

#include <core/base/utils/UUIDUtil.hpp>
#include <core/base/common/manager/UUIDManager.hpp>
#include <runtime/base/RuntimeApplication.hpp>
#include "Scene.hpp"

namespace assets::scene
{
    Scene::Scene(const std::shared_ptr<base::UUID> &existingUuid,
                 bool init, std::shared_ptr<io::YamlConfiguration> &yml, const std::shared_ptr<ISkyBox> &skybox_, std::string name)
            : base::Object(existingUuid),
              IMetaAccessor(yml, init, existingUuid),
              name(std::move(name))
    {
        root = std::make_shared<SceneGroup>(base::UUIDManager::getUUID(utils::uuidUtil::getUUID(), false), false, yml);
        environmentLight = std::make_shared<LightEntity>(base::UUIDManager::getUUID(utils::uuidUtil::getUUID(), false),
                                                         false, yml);
        mainCameraEntity = std::make_shared<CameraEntity>(base::UUIDManager::getUUID(utils::uuidUtil::getUUID(), false),
                                                          false, yml, Camera(), "testCamera",
                                                          runtimeApp.getScreenWidth(), runtimeApp.getScreenHeight());

        skybox = std::make_shared<SkyboxEntity>(base::UUIDManager::getUUID(utils::uuidUtil::getUUID(), false), false, yml, skybox_);

        root->addChild(environmentLight);
        root->addChild(mainCameraEntity);

        setMainCameraEntity(mainCameraEntity);
    }

    bool Scene::addChildToNode(const std::shared_ptr<base::UUID> &parentUuid, const std::shared_ptr<Object> &child)
    {
        auto parentNode = findNodeByUUID(root, parentUuid);
        if (parentNode != nullptr)
        {
            auto sceneGroupParent = std::dynamic_pointer_cast<SceneGroup>(parentNode);
            if (sceneGroupParent)
            {
                sceneGroupParent->addChild(child);
                return true;
            }
        }
        return false;
    }

    bool Scene::removeChildFromNode(const std::shared_ptr<base::UUID> &uuid)
    {
        return removeChildFromNodeRecursive(root, uuid);
    }

    bool Scene::removeChildFromNodeRecursive(const std::shared_ptr<SceneGroup> &node,
                                             const std::shared_ptr<base::UUID> &uuid)
    {
        if (node->removeChildByUUID(uuid))
        {
            return true;
        }
        for (auto &child: node->getChildren())
        {
            auto childGroup = std::dynamic_pointer_cast<SceneGroup>(child);
            if (childGroup && removeChildFromNodeRecursive(childGroup, uuid))
            {
                return true;
            }
        }
        return false;
    }

    bool Scene::updateNode(const std::shared_ptr<base::UUID> &uuid, const std::shared_ptr<Object> &newNode)
    {
        return updateNodeRecursive(root, uuid, newNode);
    }

    bool Scene::updateNodeRecursive(const std::shared_ptr<SceneGroup> &node, const std::shared_ptr<base::UUID> &uuid,
                                    const std::shared_ptr<Object> &newNode)
    {
        if (node->updateChildByUUID(uuid, newNode))
        {
            return true;
        }
        for (auto &child: node->getChildren())
        {
            auto childGroup = std::dynamic_pointer_cast<SceneGroup>(child);
            if (childGroup && updateNodeRecursive(childGroup, uuid, newNode))
            {
                return true;
            }
        }
        return false;
    }

    std::shared_ptr<Object>
    Scene::findNodeByUUID(const std::shared_ptr<SceneGroup> &node, const std::shared_ptr<base::UUID> &uuid) const
    {
        if (node->getUUID() == uuid)
        {
            return node;
        }
        for (auto &child: node->getChildren())
        {
            if (child->getUUID() == uuid)
            {
                return child;
            }
            auto childGroup = std::dynamic_pointer_cast<SceneGroup>(child);
            if (childGroup)
            {
                auto found = findNodeByUUID(childGroup, uuid);
                if (found != nullptr)
                {
                    return found;
                }
            }
        }
        return nullptr;
    }

    const std::shared_ptr<SkyboxEntity> &Scene::getSkybox() const
    {
        return skybox;
    }

    void Scene::setSkybox(const std::shared_ptr<SkyboxEntity> &skybox_)
    {
        Scene::skybox = skybox_;
    }

    const std::shared_ptr<SceneGroup> &Scene::getRoot() const
    {
        return root;
    }

    void Scene::setRoot(const std::shared_ptr<SceneGroup> &root_)
    {
        Scene::root = root_;
    }

    const std::shared_ptr<LightEntity> &Scene::getEnvironmentLight() const
    {
        return environmentLight;
    }

    void Scene::setEnvironmentLight(const std::shared_ptr<LightEntity> &environmentLight_)
    {
        Scene::environmentLight = environmentLight_;
    }

    const std::shared_ptr<CameraEntity> &Scene::getMainCameraEntity() const
    {
        return mainCameraEntity;
    }

    void Scene::setMainCameraEntity(const std::shared_ptr<CameraEntity> &mainCameraEntity_)
    {
        Scene::mainCameraEntity = mainCameraEntity_;
    }

    const std::vector<std::shared_ptr<CameraEntity>> &Scene::getCameraEntityList() const
    {
        return cameraEntityList;
    }

    void Scene::setCameraEntityList(const std::vector<std::shared_ptr<CameraEntity>> &cameraEntityList_)
    {
        Scene::cameraEntityList = cameraEntityList_;
    }

    void Scene::update()
    {
        updateNode(root);
    }

    void Scene::setName(const std::string &name_)
    {
        name = name_;
    }

    std::string Scene::getName() const
    {
        return name;
    }

    void Scene::addCameraEntity(const std::shared_ptr<CameraEntity> &cameraEntity)
    {
        cameraEntityList.push_back(cameraEntity);
    }

    std::string Scene::serialize() const
    {
        return std::string();
    }

    void Scene::deserialize(const std::string &data)
    {

    }

    void Scene::beforeRendering(const std::vector<std::any> &params)
    {
        if(skybox)
        {
            skybox->beforeRendering({});
        }
        if (root)
        {
            traverseAndUpdate(root, params);
        }
    }

    void Scene::traverseAndUpdate(const std::shared_ptr<base::Object> &node, const std::vector<std::any> &params)
    {
        // 尝试将当前节点动态转换为 IRenderUpdatable 接口
        auto renderUpdatable = std::dynamic_pointer_cast<base::IRenderUpdatable>(node);
        if (renderUpdatable)
        {
            renderUpdatable->beforeRendering(params);
        }

        // 检查当前节点是否为 SceneGroup 类型
        auto sceneGroup = std::dynamic_pointer_cast<assets::scene::SceneGroup>(node);
        if (sceneGroup)
        {
            const auto &children = sceneGroup->getChildren();
            for (const auto &child: children)
            {
                traverseAndUpdate(child, params);
            }
        }
    }

    void Scene::afterRendering(const std::vector<std::any> &params)
    {

    }

    void Scene::addChild(const std::shared_ptr<Object> &child)
    {
        root->addChild(child);
    }

    void Scene::updateNode(const std::shared_ptr<Object> &node)
    {
        std::shared_ptr<base::Updatable> updatable = std::dynamic_pointer_cast<base::Updatable>(node);
        if (updatable)
        {
            updatable->update();
        }

        std::shared_ptr<SceneGroup> group = std::dynamic_pointer_cast<SceneGroup>(node);
        if (group)
        {
            const auto &children = group->getChildren();
            for (const auto &child: children)
            {
                updateNode(child);
            }
        }
    }

} // assets