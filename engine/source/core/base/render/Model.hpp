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
#include <core/base/interface/IMetaAccessor.hpp>
#include <core/base/common/manager/UUIDManager.hpp>
#include "core/base/interface/IRenderable.hpp"
#include "core/base/interface/Interface.hpp"
#include "core/base/interface/ITransformable.hpp"
#include "core/base/common/Transform.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"
#include "Bone.hpp"

namespace base
{
    // 前向声明
    class Mesh;

    class Material;

    class Texture;

    struct Node
    {
        std::string name;
        std::vector<std::shared_ptr<Mesh>> meshes;
        std::vector<std::shared_ptr<Node>> children;

        explicit Node(std::string nodeName = "") : name(std::move(nodeName))
        {}
    };

    class ModelBlockOperator : implements BlockOperator
    {
        void writeToBlock(const std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml)
        {
            auto materialsMap = std::any_cast<std::map<std::shared_ptr<UUID>, std::shared_ptr<UUID>>>(metadata->getValue("materials"));
            std::map<std::string, std::string> materials;
            for (const auto& pair : materialsMap) {
                materials[pair.first->toString()] = pair.second->toString();
            }
            yml->set("ModelBlockOperator.materials", materials);
        }

        void readFromBlock(const std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml)
        {
            auto materialsNode = yml->getNode("ModelBlockOperator.materials");
            std::map<std::shared_ptr<UUID>, std::shared_ptr<UUID>> materialsUUIDMap;

            for (auto it = materialsNode.begin(); it != materialsNode.end(); ++it) {
                auto keyStr = it->first.as<std::string>();
                auto valueStr = it->second.as<std::string>();

                auto keyUuid = UUIDManager::getUUID(keyStr);
                auto valueUuid = UUIDManager::getUUID(valueStr);
                materialsUUIDMap[keyUuid] = valueUuid;
            }

            metadata->setValue("materials", materialsUUIDMap);
        }

        void initBlock(std::shared_ptr<Metadata> &metadata, const std::vector<std::any> &params)
        {
            if (!params.empty() && params[0].type() == typeid(std::map<std::string, std::string>))
            {
                auto materialsStrMap = std::any_cast<std::map<std::string, std::string>>(params[0]);
                std::map<std::shared_ptr<UUID>, std::shared_ptr<UUID>> materialsMap;
                for (const auto &pair: materialsStrMap)
                {
                    materialsMap[UUIDManager::getUUID(pair.first)] = UUIDManager::getUUID(pair.second);
                }
                metadata->setValue("materials", materialsMap);
            }
            else
            {
                metadata->setValue("materials", std::map<std::shared_ptr<UUID>, std::shared_ptr<UUID>>{});
            }
        }
    };

    class Model
            : implements IRenderable,
              implements Object,
              implements std::enable_shared_from_this<Model>,
              implements ITransformableUpdate,
              implements INameable,
              implements IMetaAccessor
    {
    private:
        std::string name;
        std::shared_ptr<Node> rootNode;
        std::unordered_map<std::string, BoneInfo> bonesInfo;
        unsigned int boneCount = 0;

    public:
        explicit Model(const std::shared_ptr<base::UUID> &existingUuid,
                       bool init,
                       std::string modelName,
                       const std::shared_ptr<Node> &root,
                       std::shared_ptr<io::YamlConfiguration> &yml,
                       const Transform &initialTransform = Transform(),
                       const std::shared_ptr<base::UUID> &shaderUUID = nullptr,
                       const std::shared_ptr<base::UUID> &materialUUID = nullptr);

        void init()
        {
            addOperator(std::make_shared<ModelBlockOperator>());
        }

        std::shared_ptr<Node> &getRootNode();

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        void beforeRendering(const std::vector<std::any> &params) override;

        void afterRendering(const std::vector<std::any> &params) override;

        void updateGlobalTransform(std::vector<Transform> &additionalTransforms) override;

        void getRenderData(std::vector<RenderData> &renderDataList) override;

        [[nodiscard]] Transform getLocalTransform() const override;

        std::shared_ptr<Mesh> getMesh(const std::shared_ptr<base::UUID> &uuidPtr);

        void bindMeshesToModel(const std::shared_ptr<Node> &node);

        void updateAllMeshShader();
    private:
        void processNode(const std::shared_ptr<Node> &node, std::vector<RenderData> &renderDataList);

        void updateObservedGlobalTransform(std::vector<Transform> &additionalTransforms) override;
    };
}

#endif //DEMONENGINE_MODEL_HPP