//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_MODEL_HPP
#define DEMONENGINE_MODEL_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/interface/ITransformable.hpp"
#include "core/base/common/Transform.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/IRenderable.hpp"
#include "Texture.hpp"
#include "Bone.hpp"
#include "core/base/interface/INameable.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

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
        std::shared_ptr<Node> rootNode;
        std::string directory;
        std::unordered_map<std::string, std::shared_ptr<Texture>> texturesLoaded;
        std::unordered_map<std::string, BoneInfo> bonesInfo;
        unsigned int boneCount = 0;

    public:
        explicit Model(const std::string &modelPath, const std::string &modelName,
                       const std::unordered_map<std::string, std::shared_ptr<Texture>> &textures,
                       const std::shared_ptr<Node> &root, const Transform &initialTransform = Transform());

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        void updateTransformsBeforeRendering() override;

        void updateActualTransform(std::vector<Transform> &additionalTransforms) override;

        void updateObservedActualTransform(std::vector<Transform> &additionalTransforms) override;

        RenderData getRenderData(Transform combinedTransform) override;

        [[nodiscard]] Transform getLocalTransform() const override;

        std::shared_ptr<Mesh> getMesh(const std::string &meshName);

        void bindMeshesToModel(const std::shared_ptr<Node> &node);
    };
}

#endif //DEMONENGINE_MODEL_HPP