//
// Created by Demon on 2024/3/6.
//
#ifndef DEMONENGINE_MODELMANAGER_HPP
#define DEMONENGINE_MODELMANAGER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileManager.hpp"
#include "TextureManager.hpp"
#include "core/base/render/Model.hpp"
#include "core/base/render/Mesh.hpp"
#include "core/base/render/Vertex.hpp"
#include "AssetsMainManager.hpp"
#include <vector>
#include <any>
#include <optional>
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include "assimp/scene.h"

namespace assets
{
    class ModelManager : implements IFileManager
    {
    private:
        static std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Model>> loadedModels;
    public:
        ModelManager();

        /**
         * @param params[0] 资源目录根路径
         * @param params[1]
         * */
        std::optional<std::shared_ptr<base::UUID>> loadResource(const std::vector<std::any> &params) override;

        void unloadResource(const std::vector<std::any> &params) override;

        [[nodiscard]] bool isResourceLoaded(const std::vector<std::any> &params) const override;

        void updateResource(const std::vector<std::any> &params) override;

        std::optional<std::shared_ptr<base::Model>> getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr);

        void updateModelRelation(const std::shared_ptr<base::UUID> &uuid_ptr);

    private:
        std::shared_ptr<base::Model> loadModel(const std::string &path, const std::shared_ptr<base::UUID> &existingUuid,
                                               bool init, std::shared_ptr<io::YamlConfiguration> &yml);

        void processNode(const std::shared_ptr<base::Node> &node, aiNode *aiNode, const aiScene *scene, int &meshIndex,
                         const std::string &directory, const base::Transform &parentTransform,
                         const std::string &pathIdentifier);

        std::shared_ptr<base::Mesh> processMesh(aiMesh *mesh, const aiScene *scene, const std::string &meshName,
                                                const std::string &rootPath, const base::Transform &nodeTransform);

        static base::Transform convertAiMatrixToTransform(const aiMatrix4x4 &aiMatrix);
    };
}

#endif //DEMONENGINE_MODELMANAGER_HPP
