//
// Created by Demon on 2023/7/7.
//

#ifndef DEMONENGINE_CAMERA_HPP
#define DEMONENGINE_CAMERA_HPP

#include "core/base/lib/pch.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "core/base/common/Transform.hpp"

namespace base
{
    enum CameraMovement
    {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float DEFAULT_ZOOM = 45.0f;

    class Camera
    {
    public:
        Transform transform;
        glm::vec3 front;
        glm::vec3 right;
        glm::vec3 up;
        float movementSpeed;
        float mouseSensitivity;
        float zoom;
        float fov;
        float aspectRatio = 16.0f / 9.0f;  // 默认纵横比
        float nearPlane = 0.1f;  // 默认近裁剪面
        float farPlane = 100.0f;  // 默认远裁剪面

        Camera(glm::vec3 position = glm::vec3(0, 0, 3), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
                                                       mouseSensitivity(SENSITIVITY), zoom(DEFAULT_ZOOM),
                                                       fov(DEFAULT_ZOOM)
        {
            transform.position = position;
            transform.rotation = glm::quat(glm::vec3(pitch, yaw, 0.0f));
            updateCameraVectors();
        }

        glm::mat4 getProjectionMatrix()
        {
            return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        }

        glm::mat4 getViewMatrix()
        {
            return glm::lookAt(transform.position, transform.position + front, up);
        }

        void processKeyboard(CameraMovement direction, float deltaTime)
        {
            float velocity = movementSpeed * deltaTime;
            if (direction == FORWARD)
                transform.position += front * velocity;
            if (direction == BACKWARD)
                transform.position -= front * velocity;
            if (direction == LEFT)
                transform.position -= right * velocity;
            if (direction == RIGHT)
                transform.position += right * velocity;
        }

        void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true)
        {
            xOffset *= mouseSensitivity;
            yOffset *= mouseSensitivity;
            glm::vec3 euler = glm::eulerAngles(transform.rotation) +
                              glm::vec3(glm::radians(-yOffset), glm::radians(xOffset), 0.0f);
            euler.x = glm::clamp(euler.x, glm::radians(-89.0f), glm::radians(89.0f));
            transform.rotation = glm::quat(euler);

            updateCameraVectors();
        }

        void processMouseScroll(float yOffset)
        {
            zoom = std::max(1.0f, std::min(zoom - yOffset, 45.0f));
            fov = DEFAULT_ZOOM / zoom;  // 根据缩放比例调整视场角
        }

        void updateCameraVectors()
        {
            glm::vec3 euler = glm::eulerAngles(transform.rotation);
            front.x = cos(euler.y) * cos(euler.x);
            front.y = sin(euler.x);
            front.z = sin(euler.y) * cos(euler.x);
            front = glm::normalize(front);
            right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
            up = glm::normalize(glm::cross(right, front));
        }
    };
}
#endif //DEMONENGINE_CAMERA_HPP