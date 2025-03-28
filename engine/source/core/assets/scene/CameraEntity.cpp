﻿//
// Created by Demon on 2024/3/28.
//

#include <runtime/base/RuntimeApplication.hpp>
#include "CameraEntity.hpp"

namespace assets::scene
{
    CameraEntity::CameraEntity(const std::shared_ptr<base::UUID> &existingUuid,
                               bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                               const Camera &camera_, std::string name, int width, int height) :
            base::Object(existingUuid),
            IMetaAccessor(yml, init, existingUuid),
            camera(camera_),
            name(std::move(name))
    {
        lastX = width / 2.0f;
        lastY = height / 2.0f;
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
        cameraSpeed = speed;
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

    std::string CameraEntity::serialize() const
    {
        boost::property_tree::ptree pt;

        // 基本属性
        pt.put("name", name);

        // 相机属性
        pt.put("camera.position", std::to_string(camera.transform.position.x) + " " +
                                  std::to_string(camera.transform.position.y) + " " +
                                  std::to_string(camera.transform.position.z));

        auto quat = camera.transform.rotation;
        pt.put("camera.rotation", std::to_string(quat.w) + " " +
                                  std::to_string(quat.x) + " " +
                                  std::to_string(quat.y) + " " +
                                  std::to_string(quat.z));

        pt.put("camera.movementSpeed", camera.movementSpeed);
        pt.put("camera.mouseSensitivity", camera.mouseSensitivity);
        pt.put("camera.zoom", camera.zoom);

        std::ostringstream buf;
        boost::property_tree::write_json(buf, pt, true);
        return buf.str();
    }

    void CameraEntity::deserialize(const std::string &data)
    {
        std::istringstream is(data);
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(is, pt);

        name = pt.get<std::string>("name");

        std::istringstream positionStream(pt.get<std::string>("camera.position"));
        positionStream >> camera.transform.position.x >> camera.transform.position.y >> camera.transform.position.z;

        std::istringstream rotationStream(pt.get<std::string>("camera.rotation"));
        rotationStream >> camera.transform.rotation.w >> camera.transform.rotation.x
                       >> camera.transform.rotation.y >> camera.transform.rotation.z;

        camera.movementSpeed = pt.get<float>("camera.movementSpeed");
        camera.mouseSensitivity = pt.get<float>("camera.mouseSensitivity");
        camera.zoom = pt.get<float>("camera.zoom");

        camera.updateCameraVectors();
    }

    glm::mat4 CameraEntity::getProjectionMatrix()
    {
        return camera.getProjectionMatrix();
    }

    glm::mat4 CameraEntity::getViewMatrix()
    {
        return camera.getViewMatrix();
    }

    void CameraEntity::update()
    {
        camera.updateState(base::Application::getDeltaTime());
    }

    void CameraEntity::processMouseMovement(float currentX, float currentY)
    {
        if (firstMouse)
        {
            lastX = currentX;
            lastY = currentY;
            firstMouse = false;
        }

        float xOffset = currentX - lastX;
        float yOffset = lastY - currentY;

        lastX = currentX;
        lastY = currentY;

        camera.processMouseMovement(xOffset, yOffset);
    }

    void CameraEntity::processKeyboard(CameraMovement direction, bool isPressed)
    {
        camera.processKeyboard(direction, isPressed);
    }

    void CameraEntity::setCameraMovementState(bool flag)
    {
        if (flag)camera.movementSpeed = cameraSpeed;
        else camera.movementSpeed = 0.0f;
    }

}