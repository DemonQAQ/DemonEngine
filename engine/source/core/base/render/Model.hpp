//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_MODEL_HPP
#define DEMONENGINE_MODEL_HPP

#include "core/base/lib/pch.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "core/base/interface/Interface.hpp"
#include "core/base/interface/ITransformable.hpp"
#include "core/base/common/Transform.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/IRenderable.hpp"
#include "Texture.hpp"
#include "Bone.hpp"
#include "core/base/interface/INameable.hpp"
#include "Material.hpp"

namespace base
{

    // 前向声明
    class Mesh;

    struct Node
    {
        std::string name;
        std::vector<std::shared_ptr<Mesh>> meshes;
        std::vector<std::shared_ptr<Node>> children;

        explicit Node(std::string nodeName = "") : name(std::move(nodeName))
        {}
    };

    class Model : implements Object, IRenderable, std::enable_shared_from_this<Model>, ITransformableUpdate, INameable
    {
    private:
        std::string name;
        std::string directory;
        std::shared_ptr<Node> rootNode;
        std::unordered_map<std::string, BoneInfo> bonesInfo;
        unsigned int boneCount = 0;

    public:
        explicit Model(const std::string &modelPath, const std::string &modelName,
                       const std::shared_ptr<Node> &root, const Transform &initialTransform = Transform(),
                       UUID *shaderUUID = nullptr, UUID *materialUUID = nullptr);

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        void beforeRendering(const std::vector<std::any> &params) override;

        void afterRendering(const std::vector<std::any> &params) override;

        void updateGlobalTransform(std::vector<Transform> &additionalTransforms) override;

        void updateObservedGlobalTransform(std::vector<Transform> &additionalTransforms) override;

        void getRenderData(std::vector<RenderData> renderDataList) override;

        [[nodiscard]] Transform getLocalTransform() const override;

        std::shared_ptr<Mesh> getMesh(const std::string &meshName);

        void bindMeshesToModel(const std::shared_ptr<Node> &node);

    private:
        void processNode(const std::shared_ptr<Node>& node, std::vector<RenderData>& renderDataList);
    };
}

#endif //DEMONENGINE_MODEL_HPP