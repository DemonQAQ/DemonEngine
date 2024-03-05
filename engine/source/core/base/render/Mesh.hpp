//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_MESH_HPP
#define DEMONENGINE_MESH_HPP

#include <utility>
#include <vector>
#include <string>
#include "Texture.hpp"
#include "Vertex.hpp"
#include "core/base/interface/Interface.hpp"
#include "core/base/interface/ITransformable.hpp"
#include "core/base/common/Transform.hpp"
#include "core/base/interface/IRenderable.hpp"
#include "Model.hpp"
#include "core/base/interface/INameable.hpp"

namespace base
{
    class Mesh : implements IRenderable, ITransformableUpdate, INameable
    {
    private:
        std::string name;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        Transform transform;
        std::weak_ptr<Model> fatherModel;

    public:
        Mesh(std::string name, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
             const std::vector<Texture> &textures, const base::Transform &initialTransform = base::Transform())
                : name(std::move(name)), vertices(vertices), indices(indices), textures(textures)
        {
            setTransform(initialTransform);
        }

        void setName(const std::string &name_) override
        {
            this->name = name_;
        }

        [[nodiscard]] std::string getName() const override
        {
            return name;
        }

        void updateTransformsBeforeRendering() override
        {

        }

        void updateActualTransform(std::vector<Transform> &additionalTransforms) override
        {
            updateSelfActualTransform(additionalTransforms);
        }

        void updateObservedActualTransform(std::vector<Transform> &additionalTransforms) override
        {}

        RenderData getRenderData(Transform combinedTransform) override
        {

        }

        void setFatherModel(const std::shared_ptr<Model> &model)
        {
            fatherModel = model;
        }

        [[nodiscard]] Transform getLocalTransform() const override
        {
            return getTransform();
        }
    };
}

#endif //DEMONENGINE_MESH_HPP
