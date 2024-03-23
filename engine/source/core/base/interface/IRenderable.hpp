//
// Created by Demon on 2024/3/2.
//

#ifndef DEMONENGINE_IRENDERABLE_HPP
#define DEMONENGINE_IRENDERABLE_HPP

#include "core/base/common/Transform.hpp"
#include "Interface.hpp"
#include "core/base/render/Vertex.hpp"
#include "core/base/render/Material.hpp"

namespace base
{
    struct RenderData
    {
        Transform globalTransform;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        base::UUID *material;
        base::UUID *useShader;

        RenderData() : globalTransform(Transform()), material(nullptr), useShader(nullptr)
        {}
    };

    interface IRenderable
    {
    private:
        Transform actualTransform;
        base::UUID *useShader;
        base::UUID *material;

    public:
        IRenderable() : useShader(nullptr), material(nullptr)
        {}

        virtual void getRenderData(std::vector<RenderData> renderDataList) = 0;

        void bindMaterial(base::UUID *uuid)
        {
            this->material = uuid;
        }

        void unbindMaterial()
        {
            this->material = nullptr;
        }

        bool usingMaterial()
        {
            return this->material != nullptr;
        }

        base::UUID *getMaterial()
        {
            return this->material;
        }

        void bindShader(base::UUID *uuid)
        {
            this->useShader = uuid;
        }

        void unbindShader()
        {
            this->useShader = nullptr;
        }

        bool usingShader()
        {
            return this->useShader != nullptr;
        }

        base::UUID *getShader()
        {
            return this->useShader;
        }

        /**
         * 调用此方法更新自身和相关的实例的状态
         * */
        virtual void updateGlobalTransform(std::vector<Transform> &additionalTransforms) = 0;

        [[nodiscard]] virtual Transform getLocalTransform() const = 0;

        [[nodiscard]] const Transform &getGlobalTransform() const
        {
            return actualTransform;
        }

    protected:
        static UUID *getDefaultShader()
        {
            return nullptr;
        }

        static UUID *getDefaultMaterial()
        {
            return nullptr;
        }

        void updateSelfGlobalTransform(const std::vector<Transform> &additionalTransforms)
        {
            std::vector<Transform> transformsToMerge = {getLocalTransform()};
            transformsToMerge.insert(transformsToMerge.end(), additionalTransforms.begin(), additionalTransforms.end());
            actualTransform = Transform::merge(transformsToMerge);
        }

        virtual void updateObservedGlobalTransform(std::vector<Transform> &additionalTransforms) = 0;
    };

}

#endif //DEMONENGINE_IRENDERABLE_HPP
