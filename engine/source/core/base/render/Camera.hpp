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
        RIGHT,
        NONE
    };

    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float DEFAULT_ZOOM = 45.0f;

    class Camera
    {
    private:
        bool movingForward = false;
        bool movingBackward = false;
        bool movingLeft = false;
        bool movingRight = false;

        bool constrainPitch = true;
    public:
        Transform transform;
        glm::vec3 front;
        glm::vec3 right;
        glm::vec3 up;
        glm::vec3 worldUp;
        float movementSpeed;
        float mouseSensitivity;
        float yaw;
        float pitch;
        float zoom;
        float fov;
        float aspectRatio = 16.0f / 9.0f;  // 默认纵横比
        float nearPlane = 0.1f;  // 默认近裁剪面
        float farPlane = 100.0f;  // 默认远裁剪面

        Camera(glm::vec3 position = glm::vec3(0, 1, 2), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               float yaw_ = YAW, float pitch_ = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED),
                                                         mouseSensitivity(SENSITIVITY), zoom(DEFAULT_ZOOM),
                                                         fov(DEFAULT_ZOOM), yaw(yaw_), pitch(pitch_)
        {
            transform.position = position;
            worldUp = up;
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

        void updateCameraVectors()
        {
            glm::vec3 front_;
            front_.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front_.y = sin(glm::radians(pitch));
            front_.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            front = glm::normalize(front_);

            right = glm::normalize(glm::cross(front, worldUp));
            up = glm::normalize(glm::cross(right, front));
        }

        void setConstrainPitch(bool constrainPitch_)
        {
            constrainPitch = constrainPitch_;
        }

        void updateState(double deltaTime)
        {
            double velocity = movementSpeed * deltaTime;
            if (movingForward)
            {
                transform.position += front * (float) velocity;
            }
            if (movingBackward)
            {
                transform.position -= front * (float) velocity;
            }
            if (movingLeft)
            {
                transform.position -= right * (float) velocity;
            }
            if (movingRight)
            {
                transform.position += right * (float) velocity;
            }

            if (constrainPitch)
            {
                pitch = glm::clamp(pitch, -89.0f, 89.0f);
            }
            updateCameraVectors();
        }

        void processMouseMovement(float xOffset, float yOffset)
        {
            xOffset *= mouseSensitivity;
            yOffset *= mouseSensitivity;

            yaw += xOffset;
            pitch += yOffset;

            if (constrainPitch)
            {
                pitch = glm::clamp(pitch, -89.0f, 89.0f);
            }

            updateCameraVectors();
        }

        void processKeyboard(CameraMovement direction, bool isPressed)
        {
            switch (direction)
            {
                case FORWARD:
                    movingForward = isPressed;
                    break;
                case BACKWARD:
                    movingBackward = isPressed;
                    break;
                case LEFT:
                    movingLeft = isPressed;
                    break;
                case RIGHT:
                    movingRight = isPressed;
                    break;
                case NONE:
                    break;
            }
        }

        void processScroll(float xOffset, float yOffset)
        {
            if (yOffset != 0)
            {
                fov -= yOffset * 0.05f;
                fov = glm::clamp(fov, 1.0f, 90.0f);
            }
        }
    };
}
#endif //DEMONENGINE_CAMERA_HPP