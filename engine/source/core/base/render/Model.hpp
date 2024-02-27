//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_MODEL_HPP
#define DEMONENGINE_MODEL_HPP

#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

namespace base
{
    class Model
    {
    public:
        std::vector<Mesh> meshes;
        std::string directory;
        std::unordered_map<std::string, std::shared_ptr<Texture>> textures_loaded;

        Model(const std::string &path)
        {
            loadModel(path);
        }

    private:
        void loadModel(const std::string &path)
        {
            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                                           aiProcess_CalcTangentSpace);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
                return;
            }

            directory = path.substr(0, path.find_last_of('/'));
            processNode(scene->mRootNode, scene);
        }

        void processNode(aiNode *node, const aiScene *scene)
        {
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }

            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }
        }

        Mesh processMesh(aiMesh *mesh, const aiScene *scene)
        {
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<Texture> textures;
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                glm::vec3 vector;

                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.position = vector;

                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.normal = vector;
                }

                if (mesh->mTextureCoords[0])
                {
                    glm::vec2 vec;
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.texCoords = vec;
                } else
                {
                    vertex.texCoords = glm::vec2(0.0f, 0.0f);
                }

                vertices.push_back(vertex);
            }

            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                {
                    indices.push_back(face.mIndices[j]);
                }
            }

            for (aiTextureType aiType: supportedAiTextureTypes)
            {
                TextureType type = aiTextureTypeToTextureType(aiType);
                if (type == TextureType::Unknown)continue;
                loadMaterialTextures(material, aiType, toString(type));
            }

            return Mesh(vertices, indices, textures);
        }

        void loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName)
        {
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                // 检查纹理是否已加载，如果已加载，则继续使用已加载的纹理
                if (textures_loaded.find(str.C_Str()) == textures_loaded.end())
                {
                    auto texture = std::make_shared<Texture>(
                            Texture::loadTextureType(str.C_Str(), directory, fromStringToTextureType(typeName)));
                    textures_loaded[str.C_Str()] = texture;
                }
            }
        }
    };
}

#endif //DEMONENGINE_MODEL_HPP