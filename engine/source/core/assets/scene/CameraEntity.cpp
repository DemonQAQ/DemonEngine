//
// Created by Demon on 2024/3/28.
//

#include "CameraEntity.hpp"

namespace assets::scene
{
    CameraEntity::CameraEntity(std::string name) : base::Object(), name(std::move(name))
    {

    }

    CameraEntity::CameraEntity(const std::string &uuidStr, const Camera &camera_, std::string name) : base::Object(
            uuidStr), name(std::move(name)), camera(camera_)
    {

    }

    void CameraEntity::setName(const std::string &name_)
    {
        name = name_;
    }

    std::string CameraEntity::getName() const
    {
        return name;
    }

    void CameraEntity::beforeRendering(const std::vector<std::any> &params)
    {

    }

    void CameraEntity::afterRendering(const std::vector<std::any> &params)
    {

    }

    void CameraEntity::setCameraPosition(const glm::vec3 &position)
    {
        camera.transform.position = position;
    }

    void CameraEntity::setCameraRotation(const glm::quat &rotation)
    {
        camera.transform.rotation = rotation;
    }

    void CameraEntity::setCameraMovementSpeed(float speed)
    {
        camera.movementSpeed = speed;
    }

    void CameraEntity::setCameraMouseSensitivity(float sensitivity)
    {
        camera.mouseSensitivity = sensitivity;
    }

    void CameraEntity::setCameraZoom(float zoom)
    {
        camera.zoom = zoom;
    }

    [[nodiscard]] glm::vec3 CameraEntity::getCameraPosition() const
    {
        return camera.transform.position;
    }

    [[nodiscard]] glm::quat CameraEntity::getCameraRotation() const
    {
        return camera.transform.rotation;
    }

    [[nodiscard]] float CameraEntity::getCameraMovementSpeed() const
    {
        return camera.movementSpeed;
    }

    [[nodiscard]] float CameraEntity::getCameraMouseSensitivity() const
    {
        return camera.mouseSensitivity;
    }

    [[nodiscard]] float CameraEntity::getCameraZoom() const
    {
        return camera.zoom;
    }

}