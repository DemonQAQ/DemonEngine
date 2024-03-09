//
// Created by Demon on 2024/3/4.
//

#ifndef DEMONENGINE_ITRANSFORMABLEUPDATE_HPP
#define DEMONENGINE_ITRANSFORMABLEUPDATE_HPP

#include "Interface.hpp"
#include "ITransformable.hpp"
#include "core/base/common/Transform.hpp"

namespace base
{
    interface ITransformableUpdate : implements ITransformable
    {
    protected:
        bool transformDirty = false;
    public:
        Transform transform;

        ~ITransformableUpdate() override = default;

        virtual void updateTransformsBeforeRendering() = 0;

        [[nodiscard]] bool isTransformDirty() const
        {
            return transformDirty;
        }

        void setTransform(const Transform &newTransform)
        {
            if (transform == newTransform)return;
            transform = newTransform;
            setTransformDirty(true);
        }

        [[nodiscard]] Transform getTransform() const
        {
            return transform;
        }

        void setTransformDirty(bool flag)
        {
            transformDirty = flag;
        }

        void setPosition(const glm::vec3 &newPosition) override
        {
            if (transform.position == newPosition) return;
            transform.position = newPosition;
            setTransformDirty(true);
        }

        [[nodiscard]] glm::vec3 getPosition() const override
        {
            return transform.position;
        }

        void setRotation(const glm::quat &newRotation) override
        {
            if (transform.rotation == newRotation)return;
            transform.rotation = newRotation;
            setTransformDirty(true);
        }

        [[nodiscard]] glm::quat getRotation() const override
        {
            return transform.rotation;
        }

        void setScale(const glm::vec3 &newScale) override
        {
            if (transform.scale == newScale) return;
            transform.scale = newScale;
            setTransformDirty(true);
        }

        [[nodiscard]] glm::vec3 getScale() const override
        {
            return transform.scale;
        }
    };

}

#endif //DEMONENGINE_ITRANSFORMABLEUPDATE_HPP
