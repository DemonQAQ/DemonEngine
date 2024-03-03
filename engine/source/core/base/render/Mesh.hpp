//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_MESH_HPP
#define DEMONENGINE_MESH_HPP

#include <vector>
#include <string>
#include "Texture.hpp"
#include "Vertex.hpp"
#include "core/base/interface/Interface.hpp"
#include "core/base/interface/ITransformable.hpp"
#include "core/base/common/Transform.hpp"
#include "core/base/interface/IRenderable.hpp"
#include "Model.hpp"

namespace base
{
    class Mesh : implements ITransformable, IRenderable
    {
    private:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        Transform transform;
        std::weak_ptr<Model> fatherModel;

    public:
        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
             const std::vector<Texture> &textures)
                : vertices(vertices), indices(indices), textures(textures)
        {}

        void updateActualTransform(const std::vector<Transform> &additionalTransforms) override
        {
            updateSelfActualTransform(additionalTransforms);
        }

        void updateObservedActualTransform(const std::vector<Transform> &additionalTransforms) const override
        {}

        RenderData getRenderData(Transform combinedTransform) override
        {

        }

        void setFatherModel(const std::shared_ptr<Model>& model)
        {
            fatherModel = model;
        }

        [[nodiscard]] Transform getLocalTransform() const override
        {
            return transform;
        }

        void setPosition(const glm::vec3 &position) override
        {
            transform.position = position;
        }

        [[nodiscard]] glm::vec3 getPosition() const override
        {
            return transform.position;
        }

        void setRotation(const glm::quat &rotation) override
        {
            transform.rotation = rotation;
        }

        [[nodiscard]] glm::quat getRotation() const override
        {
            return transform.rotation;
        }

        void setScale(const glm::vec3 &scale) override
        {
            transform.scale = scale;
        }

        [[nodiscard]] glm::vec3 getScale() const override
        {
            return transform.scale;
        }
    };
}

#endif //DEMONENGINE_MESH_HPP
