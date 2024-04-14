//
// Created by Demon on 2024/4/13.
//

#ifndef DEMONENGINE_ASSIMPLOADER_HPP
#define DEMONENGINE_ASSIMPLOADER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/render/Model.hpp"
#include "core/assets/interface/IFileLoader.hpp"

namespace assets
{
    class AssimpLoader : implements IFileLoader
    {
    public:
        static std::shared_ptr<base::Model> loadModel(const std::string &path);
    private:
        static std::shared_ptr<base::Model> loadModel(const std::string &path, const std::shared_ptr<base::UUID> &existingUuid,
                                               bool init, std::shared_ptr<io::YamlConfiguration> &yml);

        static void processNode(const std::shared_ptr<base::Node> &node, aiNode *aiNode, const aiScene *scene, int &meshIndex,
                         const std::string &directory, const base::Transform &parentTransform,
                         const std::string &pathIdentifier);

        static std::shared_ptr<base::Mesh> processMesh(aiMesh *mesh, const aiScene *scene, const std::string &meshName,
                                                const std::string &rootPath, const base::Transform &nodeTransform);

        static base::Transform convertAiMatrixToTransform(const aiMatrix4x4 &aiMatrix);
    };
}


#endif //DEMONENGINE_ASSIMPLOADER_HPP
