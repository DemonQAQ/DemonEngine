//
// Created by Demon on 2024/3/2.
//

#ifndef DEMONENGINE_IRENDERABLE_HPP
#define DEMONENGINE_IRENDERABLE_HPP

#include "core/base/common/Transform.hpp"

namespace base
{
    struct RenderData
    {
        //todo 根据需要添加熟数据结构
    };

    class IRenderable
    {
    public:
        virtual RenderData getRenderData(Transform transform) = 0;

        virtual void updateActualTransform(std::vector<Transform> &additionalTransforms) = 0;

        [[nodiscard]] virtual Transform getLocalTransform() const = 0;

        [[nodiscard]] const Transform &getActualTransform() const
        {
            return actualTransform;
        }

    protected:
        void updateSelfActualTransform(const std::vector<Transform> &additionalTransforms)
        {
            std::vector<Transform> transformsToMerge = {getLocalTransform()};
            transformsToMerge.insert(transformsToMerge.end(), additionalTransforms.begin(), additionalTransforms.end());
            actualTransform = Transform::merge(transformsToMerge);
        }

        virtual void updateObservedActualTransform(std::vector<Transform> &additionalTransforms) = 0;

    private:
        Transform actualTransform;
    };

}

#endif //DEMONENGINE_IRENDERABLE_HPP
