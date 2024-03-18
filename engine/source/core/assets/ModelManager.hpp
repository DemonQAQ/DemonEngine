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
#include <assimp/scene.h>

namespace assets
{
    class ModelManager : implements IFileManager
    {
    public:
        std::optional<base::UUID> LoadResource(const std::vector<std::any> &params) override;

        void UnloadResource(const std::vector<std::any> &params) override;

        [[nodiscard]] bool IsResourceLoaded(const std::vector<std::any> &params) const override;

        void UpdateResource(const std::vector<std::any> &params) override;

        std::optional<std::shared_ptr<base::Model>> GetResourceByUuid(const base::UUID &uuid);

    private:
        std::optional<base::Model> loadModel(const std::string &path);

        void processNode(const std::shared_ptr<base::Node> &node, aiNode *aiNode, const aiScene *scene, int &meshIndex,
                         const std::string &directory,
                         std::unordered_map<std::string, std::shared_ptr<base::Material>> &materialsLoaded);

        std::shared_ptr<base::Mesh>
        processMesh(aiMesh *mesh, const aiScene *scene, const std::string &meshName, const std::string &directory,
                    std::unordered_map<std::string, std::shared_ptr<base::Material>> &materialsLoaded);
    };
}

#endif //DEMONENGINE_MODELMANAGER_HPP
