//
// Created by Demon on 2024/2/29.
//

#ifndef DEMONENGINE_ITRANSFORMABLE_HPP
#define DEMONENGINE_ITRANSFORMABLE_HPP

#include "glm/vec3.hpp"
#include "glm/fwd.hpp"

namespace base
{

    interface ITransformable
    {
    public:
        virtual ~ITransformable() = default;

        virtual void setPosition(const glm::vec3 &position) = 0;

        [[nodiscard]] virtual glm::vec3 getPosition() const = 0;

        virtual void setRotation(const glm::quat &rotation) = 0;

        [[nodiscard]] virtual glm::quat getRotation() const = 0;

        virtual void setScale(const glm::vec3 &scale) = 0;

        [[nodiscard]] virtual glm::vec3 getScale() const = 0;
    };

}

#endif //DEMONENGINE_ITRANSFORMABLE_HPP
