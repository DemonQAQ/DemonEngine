//
// Created by Demon on 2024/2/29.
//

#ifndef DEMONENGINE_TRANSFORM_HPP
#define DEMONENGINE_TRANSFORM_HPP


#include <vector>
#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/detail/type_quat.hpp"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace base
{

    struct Transform
    {
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;

        Transform(const glm::vec3 &pos = glm::vec3(0.0f),
                  const glm::quat &rot = glm::quat(),
                  const glm::vec3 &scl = glm::vec3(1.0f))
                : position(pos), rotation(rot), scale(scl)
        {}

        [[nodiscard]] glm::mat4 toMatrix() const
        {
            glm::mat4 posMatrix = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 rotMatrix = glm::mat4(rotation);
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
            return posMatrix * rotMatrix * scaleMatrix; // 先缩放，后旋转，最后平移
        }

        void fromMatrix(const glm::mat4 &matrix)
        {
            position = glm::vec3(matrix[3]);
            rotation = glm::quat_cast(matrix);
            scale = glm::vec3(glm::length(matrix[0]), glm::length(matrix[1]), glm::length(matrix[2]));
        }

        static Transform merge(const std::vector<Transform> &transforms)
        {
            if (transforms.empty()) return Transform();

            auto resultMatrix = transforms[0].toMatrix();
            for (size_t i = 1; i < transforms.size(); ++i)
            {
                resultMatrix = resultMatrix * transforms[i].toMatrix();
            }

            Transform result;
            result.fromMatrix(resultMatrix);
            return result;
        }
    };

} // namespace base

#endif //DEMONENGINE_TRANSFORM_HPP
