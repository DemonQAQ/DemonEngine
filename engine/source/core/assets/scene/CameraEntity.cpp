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

    std::string CameraEntity::Serialize() const
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

    void CameraEntity::Deserialize(const std::string &data)
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

}