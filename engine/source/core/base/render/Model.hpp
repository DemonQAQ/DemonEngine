﻿//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_MODEL_HPP
#define DEMONENGINE_MODEL_HPP

#include "Mesh.hpp"
#include "Bone.hpp"
#include "core/base/interface/Interface.hpp"
#include "core/base/interface/ITransformable.hpp"
#include "core/base/common/Transform.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <unordered_map>

namespace base
{
    //todo 考虑为model和mesh实现统一接口用于在渲染时传递所需数据
    class Model : implements ITransformable
    {
    public:
        struct Node
        {
            std::string name;
            std::vector<Mesh> meshes;
            std::vector<std::shared_ptr<Node>> children;

            Node(std::string name) : name(std::move(name))
            {}
        };

        Transform transform;
        std::shared_ptr<Node> rootNode;
        std::string directory;
        std::unordered_map<std::string, std::shared_ptr<Texture>> texturesLoaded;
        std::unordered_map<std::string, BoneInfo> bonesInfo;
        unsigned int boneCount = 0;

        explicit Model(const std::string &path)
        {
            loadModel(path);
        }

        void setPosition(const glm::vec3 &position) override
        {
            transform.position = position;
        }

        [[nodiscard]] glm::vec3 getPosition() const override
        {
            return transform.position;
        }

        void setRotation(const glm::quat &rotation) override
        {
            transform.rotation = rotation;
        }

        [[nodiscard]] glm::quat getRotation() const override
        {
            return transform.rotation;
        }

        void setScale(const glm::vec3 &scale) override
        {
            transform.scale = scale;
        }

        [[nodiscard]] glm::vec3 getScale() const override
        {
            return transform.scale;
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
            rootNode = std::make_shared<Node>(scene->mRootNode->mName.C_Str());
            processNode(rootNode, scene->mRootNode, scene);
        }

        void processNode(std::shared_ptr<Node> node, aiNode *aiNode, const aiScene *scene)
        {
            for (unsigned int i = 0; i < aiNode->mNumMeshes; i++)
            {
                aiMesh *mesh = scene->mMeshes[aiNode->mMeshes[i]];
                node->meshes.push_back(processMesh(mesh, scene));
            }

            for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
            {
                std::shared_ptr<Node> childNode = std::make_shared<Node>(aiNode->mChildren[i]->mName.C_Str());
                processNode(childNode, aiNode->mChildren[i], scene);
                node->children.push_back(childNode);
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
                if (texturesLoaded.find(str.C_Str()) == texturesLoaded.end())
                {
                    auto texture = std::make_shared<Texture>(
                            Texture::loadTextureType(str.C_Str(), directory, fromStringToTextureType(typeName)));
                    texturesLoaded[str.C_Str()] = texture;
                }
            }
        }
    };
}

#endif //DEMONENGINE_MODEL_HPP