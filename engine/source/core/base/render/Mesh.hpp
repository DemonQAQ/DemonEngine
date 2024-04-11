//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_MESH_HPP
#define DEMONENGINE_MESH_HPP

#include <utility>
#include <vector>
#include <string>
#include <memory>
#include "Vertex.hpp"
#include "core/base/interface/IRenderable.hpp"
#include "core/base/interface/INameable.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"

namespace base
{
    class Model;

    class Mesh : implements IRenderable,
                 implements Object,
                 implements ITransformableUpdate,
                 implements INameable
    {
    private:
        std::string name;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::weak_ptr<Model> fatherModel;

    public:
        Mesh(const std::shared_ptr<base::UUID> &existingUuid,
             std::string name, const std::vector<Vertex> &vertices,
             const std::vector<unsigned int> &indices,
             const Transform &initialTransform = Transform(),
             const std::shared_ptr<base::UUID> &shaderUUID = nullptr,
             const std::shared_ptr<base::UUID> &materialUUID = nullptr);

        [[nodiscard]] const std::vector<Vertex> &getVertices() const;

        void setVertices(const std::vector<Vertex> &vertices_);

        [[nodiscard]] const std::vector<unsigned int> &getIndices() const;

        void setIndices(const std::vector<unsigned int> &indices_);

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        void beforeRendering(const std::vector<std::any> &params) override;

        void afterRendering(const std::vector<std::any> &params) override;

        void updateGlobalTransform(std::vector<Transform> &additionalTransforms) override;

        void updateObservedGlobalTransform(std::vector<Transform> &additionalTransforms) override;

        void getRenderData(std::vector<RenderData> renderDataList) override;

        void setFatherModel(const std::shared_ptr<Model> &model);

        [[nodiscard]] Transform getLocalTransform() const override;
    };
}

#endif //DEMONENGINE_MESH_HPP
