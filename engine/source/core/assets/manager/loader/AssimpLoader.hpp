//
// Created by Demon on 2024/4/13.
//

#ifndef DEMONENGINE_ASSIMPLOADER_HPP
#define DEMONENGINE_ASSIMPLOADER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/render/Mesh.hpp"
#include "core/base/render/Material.hpp"
#include "core/base/render/Model.hpp"
#include "core/assets/interface/IFileLoader.hpp"

namespace assets
{
    static base::TextureType aiTextureTypeToTextureType(aiTextureType aiType)
    {
        switch (aiType)
        {
            case aiTextureType_DIFFUSE:
                return base::TextureType::DIFFUSE;
            case aiTextureType_SPECULAR:
                return base::TextureType::SPECULAR;
            case aiTextureType_HEIGHT:
                return base::TextureType::HEIGHT;
            case aiTextureType_AMBIENT:
                return base::TextureType::AMBIENT_OCCLUSION;
            case aiTextureType_EMISSIVE:
                return base::TextureType::EMISSIVE;
            case aiTextureType_NORMALS:
                return base::TextureType::NORMAL;
            case aiTextureType_SHININESS:
                return base::TextureType::SHININESS;
            case aiTextureType_OPACITY:
                return base::TextureType::OPACITY;
            case aiTextureType_DISPLACEMENT:
                return base::TextureType::DISPLACEMENT;
            case aiTextureType_REFLECTION:
                return base::TextureType::REFLECTION;
            case aiTextureType_LIGHTMAP:
                return base::TextureType::LIGHT;
            default:
                return base::TextureType::DIFFUSE;
        }
    }

    static const std::vector<aiTextureType> supportedAiTextureTypes = {
            aiTextureType_DIFFUSE,           // 漫反射贴图
            aiTextureType_SPECULAR,          // 高光贴图
            aiTextureType_HEIGHT,            // 高度贴图，有时也称为位移贴图
            aiTextureType_AMBIENT,           // 环境光遮蔽贴图
            aiTextureType_EMISSIVE,          // 自发光贴图
            aiTextureType_NORMALS,           // 法线贴图
            aiTextureType_SHININESS,         // 光泽贴图
            aiTextureType_OPACITY,           // 不透明度贴图
            aiTextureType_DISPLACEMENT,      // 位移贴图
            aiTextureType_REFLECTION,        // 反射贴图
            aiTextureType_LIGHTMAP           // 光照贴图
    };

    class AssimpLoader : implements IFileLoader
    {
    public:
        static std::shared_ptr<base::Model> loadModel(const std::string &path);

    private:
        static std::shared_ptr<base::Model> loadModel(const std::string &directory, const std::string &modelName,
                                                      const std::shared_ptr<base::UUID> &existingUuid,
                                                      bool init, std::shared_ptr<io::YamlConfiguration> &yml);

        static void
        processNode(const std::shared_ptr<base::Node> &node, aiNode *aiNode, const aiScene *scene, int &meshIndex,
                    const std::string &directory, const base::Transform &parentTransform,
                    const std::string &pathIdentifier, std::vector<std::shared_ptr<base::Material>> &materials);

        static std::shared_ptr<base::Mesh>
        processMesh(aiMesh *mesh, const std::string &meshName, const std::string &rootPath,
                    const base::Transform &nodeTransform);

        static std::shared_ptr<base::Material>
        loadMaterialFromAssimp(const aiMaterial *aiMat, std::shared_ptr<io::YamlConfiguration> &metaYml,
                               const std::string &directory);

        static std::shared_ptr<base::Texture>
        loadTextureFromAssimp(const aiTextureType &aiType, const std::string &texturePath);

        static base::Transform convertAiMatrixToTransform(const aiMatrix4x4 &aiMatrix);
    };
}


#endif //DEMONENGINE_ASSIMPLOADER_HPP
