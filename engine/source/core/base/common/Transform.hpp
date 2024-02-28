//
// Created by Demon on 2024/2/29.
//

#ifndef DEMONENGINE_TRANSFORM_HPP
#define DEMONENGINE_TRANSFORM_HPP


#include "glm/vec3.hpp"
#include "glm/glm.hpp"
#include "glm/detail/type_quat.hpp"

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
    };

} // namespace base

#endif //DEMONENGINE_TRANSFORM_HPP
