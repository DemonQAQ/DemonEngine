//
// Created by Demon on 2024/3/27.
//

#include <core/base/utils/UUIDUtil.hpp>
#include "Scene.hpp"

namespace assets::scene
{
    Scene::Scene(const std::string &uuidStr, bool isUUID, std::shared_ptr<io::YamlConfiguration> &yml, std::string name)
            : base::Object(uuidStr, isUUID),
              IMetaAccessor(yml, !isUUID, uuidStr.empty() ? nullptr : std::make_shared<base::UUID>(uuidStr, isUUID)),
              name(std::move(name))
    {
        root = std::make_shared<SceneGroup>(utils::uuidUtil::getUUID(""), false, yml);
        environmentLight = std::make_shared<LightEntity>(utils::uuidUtil::getUUID(""), false, yml);
        mainCameraEntity = std::make_shared<CameraEntity>(utils::uuidUtil::getUUID(""), false, yml);
        skybox = std::make_shared<Skybox>(utils::uuidUtil::getUUID(""), false, yml);
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

    const std::shared_ptr<Skybox> &Scene::getSkybox() const
    {
        return skybox;
    }

    void Scene::setSkybox(const std::shared_ptr<Skybox> &skybox_)
    {
        Scene::skybox = skybox;
    }

    const std::shared_ptr<SceneGroup> &Scene::getRoot() const
    {
        return root;
    }

    void Scene::setRoot(const std::shared_ptr<SceneGroup> &root_)
    {
        Scene::root = root;
    }

    const std::shared_ptr<LightEntity> &Scene::getEnvironmentLight() const
    {
        return environmentLight;
    }

    void Scene::setEnvironmentLight(const std::shared_ptr<LightEntity> &environmentLight_)
    {
        Scene::environmentLight = environmentLight;
    }

    const std::shared_ptr<CameraEntity> &Scene::getMainCameraEntity() const
    {
        return mainCameraEntity;
    }

    void Scene::setMainCameraEntity(const std::shared_ptr<CameraEntity> &mainCameraEntity_)
    {
        Scene::mainCameraEntity = mainCameraEntity;
    }

    const std::vector<std::shared_ptr<CameraEntity>> &Scene::getCameraEntityList() const
    {
        return cameraEntityList;
    }

    void Scene::setCameraEntityList(const std::vector<std::shared_ptr<CameraEntity>> &cameraEntityList_)
    {
        Scene::cameraEntityList = cameraEntityList;
    }

    void Scene::update()
    {

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

    std::string Scene::Serialize() const
    {
        return std::string();
    }

    void Scene::Deserialize(const std::string &data)
    {

    }

    void Scene::beforeRendering(const std::vector<std::any> &params)
    {
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

} // assets