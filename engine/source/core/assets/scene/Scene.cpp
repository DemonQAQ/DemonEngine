//
// Created by Demon on 2024/3/27.
//

#include "Scene.hpp"

namespace assets::scene
{
    const std::shared_ptr<Skybox> &Scene::getSkybox() const
    {
        return skybox;
    }

    void Scene::setSkybox(const std::shared_ptr<Skybox> &skybox)
    {
        Scene::skybox = skybox;
    }

    const std::shared_ptr<SceneGroup> &Scene::getRoot() const
    {
        return root;
    }

    void Scene::setRoot(const std::shared_ptr<SceneGroup> &root)
    {
        Scene::root = root;
    }

    const std::shared_ptr<LightEntity> &Scene::getEnvironmentLight() const
    {
        return environmentLight;
    }

    void Scene::setEnvironmentLight(const std::shared_ptr<LightEntity> &environmentLight)
    {
        Scene::environmentLight = environmentLight;
    }

    const std::shared_ptr<CameraEntity> &Scene::getMainCameraEntity() const
    {
        return mainCameraEntity;
    }

    void Scene::setMainCameraEntity(const std::shared_ptr<CameraEntity> &mainCameraEntity)
    {
        Scene::mainCameraEntity = mainCameraEntity;
    }

    const std::vector<std::shared_ptr<CameraEntity>> &Scene::getCameraEntityList() const
    {
        return cameraEntityList;
    }

    void Scene::setCameraEntityList(const std::vector<std::shared_ptr<CameraEntity>> &cameraEntityList)
    {
        Scene::cameraEntityList = cameraEntityList;
    }

    void Scene::update()
    {

    }

    void Scene::setName(const std::string &name_)
    {

    }

    std::string Scene::getName() const
    {
        return std::string();
    }

    void Scene::load(const std::string &path)
    {

    }

    void Scene::save(const std::string &path) const
    {

    }
} // assets