//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_MESH_HPP
#define DEMONENGINE_MESH_HPP

#include <utility>
#include <vector>
#include <string>
#include <memory>
#include "Texture.hpp"
#include "Vertex.hpp"
#include "core/base/interface/IRenderable.hpp"
#include "core/base/interface/INameable.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"
#include "Material.hpp"

namespace base
{
    class Model;

    class Mesh : implements IRenderable, ITransformableUpdate, INameable
    {
    private:
        std::string name;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::shared_ptr<Material> material;
        std::weak_ptr<Model> fatherModel;

    public:
        Mesh(std::string name, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
             const std::shared_ptr<Material> &material, const Transform &initialTransform = Transform());

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        void updateTransformsBeforeRendering();

        void updateActualTransform(std::vector<Transform> &additionalTransforms) override;

        void updateObservedActualTransform(std::vector<Transform> &additionalTransforms) override;

        RenderData getRenderData(Transform combinedTransform) override;

        void setFatherModel(const std::shared_ptr<Model> &model);

        [[nodiscard]] Transform getLocalTransform() const override;
    };
}

#endif //DEMONENGINE_MESH_HPP
