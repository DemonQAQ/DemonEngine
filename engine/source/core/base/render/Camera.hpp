//
// Created by Demon on 2023/7/7.
//

#ifndef DEMONENGINE_CAMERA_HPP
#define DEMONENGINE_CAMERA_HPP

#include <glad/glad.h>
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
    const float ZOOM = 45.0f;

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

        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw = YAW, float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
                                                       mouseSensitivity(SENSITIVITY), zoom(ZOOM)
        {
            transform.position = position;
            transform.rotation = glm::quat(glm::vec3(pitch, yaw, 0.0f));
            updateCameraVectors();
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

        void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
        {
            xoffset *= mouseSensitivity;
            yoffset *= mouseSensitivity;
            glm::vec3 euler = glm::eulerAngles(transform.rotation) +
                              glm::vec3(glm::radians(-yoffset), glm::radians(xoffset), 0.0f);
            euler.x = glm::clamp(euler.x, glm::radians(-89.0f), glm::radians(89.0f));
            transform.rotation = glm::quat(euler);

            updateCameraVectors();
        }

        void processMouseScroll(float yoffset)
        {
            zoom -= (float) yoffset;
            if (zoom < 1.0f)
                zoom = 1.0f;
            if (zoom > 45.0f)
                zoom = 45.0f;
        }

    private:
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