//
// Created by Demon on 2024/3/27.
//

#include "Scene.hpp"

namespace assets::scene
{
    bool Scene::addChildToNode(const UUID &parentUuid, const std::shared_ptr<Object> &child)
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

    bool Scene::removeChildFromNode(const UUID &uuid)
    {
        return removeChildFromNodeRecursive(root, uuid);
    }

    bool Scene::removeChildFromNodeRecursive(const std::shared_ptr<SceneGroup> &node, const UUID &uuid)
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

    bool Scene::updateNode(const UUID &uuid, const std::shared_ptr<Object> &newNode)
    {
        return updateNodeRecursive(root, uuid, newNode);
    }

    bool Scene::updateNodeRecursive(const std::shared_ptr<SceneGroup> &node, const UUID &uuid,
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

    std::shared_ptr<Object> Scene::findNodeByUUID(const std::shared_ptr<SceneGroup> &node, const UUID &uuid) const
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

    Scene::Scene(std::string name) : base::Object(), name(std::move(name))
    {
        root = std::make_shared<SceneGroup>();
        environmentLight = std::make_shared<LightEntity>();
        mainCameraEntity = std::make_shared<CameraEntity>();
        skybox = std::make_shared<Skybox>();
    }

    std::string Scene::Serialize() const
    {
        return std::string();
    }

    void Scene::Deserialize(const std::string &data)
    {

    }

} // assets