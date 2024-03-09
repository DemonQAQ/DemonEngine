//
// Created by Demon on 2024/3/9.
//
#include "ModelManager.hpp"
#include <assimp/Importer.hpp>
#include <iostream>

using namespace assets;

std::optional<base::UUID> ModelManager::LoadResource(const std::vector<std::any> &params)
{
    if (params.empty() || params[0].type() != typeid(std::string))
    {
        std::cerr << "Invalid parameters for LoadResource." << std::endl;
        return std::nullopt;
    }

    std::string path;
    try
    {
        path = std::any_cast<std::string>(params[0]);
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error extracting parameters: " << e.what() << std::endl;
        return std::nullopt;
    }


    if (IsResourceLoaded(path))
    {
        std::cout << "资源已加载：" << path << std::endl;
        // You need to return the existing model's UUID here
    }

    std::optional<base::Model> model = loadModel(path);
    if (!model)
    {
        return std::nullopt;
    }

    return model->getUuid();
}

void ModelManager::UnloadResource(const std::vector<std::any> &params)
{
    // Implementation of UnloadResource
}

bool ModelManager::IsResourceLoaded(const std::string &path) const
{
    // Implementation of IsResourceLoaded
}

void ModelManager::UpdateResource(const std::string &path)
{
    // Implementation of UpdateResource
}

std::optional<base::Model> ModelManager::loadModel(const std::string &path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                                   aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return std::nullopt;
    }

    std::string directory = path.substr(0, path.find_last_of('/'));
    std::string modelName = directory.substr(directory.find_last_of('/') + 1);
    auto rootNode = std::make_shared<base::Node>(scene->mRootNode->mName.C_Str());
    int index = 0;
    std::unordered_map<std::string, std::shared_ptr<base::Texture>> texturesLoaded;

    // 假设processNode函数现在接受texturesLoaded和directory参数，以正确处理纹理
    processNode(rootNode, scene->mRootNode, scene, index, directory, texturesLoaded);

    auto model = std::make_shared<base::Model>(path, modelName, texturesLoaded, rootNode, base::Transform());
    return model ? std::optional<base::Model>(*model) : std::nullopt;
}

void ModelManager::processNode(const std::shared_ptr<base::Node> &node, aiNode *aiNode, const aiScene *scene,
                               int &meshIndex, const std::string &directory,
                               std::unordered_map<std::string, std::shared_ptr<base::Texture>> &texturesLoaded)
{
    for (unsigned int i = 0; i < aiNode->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[aiNode->mMeshes[i]];
        std::string meshName =
                mesh->mName.length > 0 ? mesh->mName.C_Str() : "mesh" + std::to_string(meshIndex++);

        auto newMesh = processMesh(mesh, scene, meshName, directory, texturesLoaded);
        node->meshes.push_back(newMesh); // 现在正确地添加了 std::shared_ptr<Mesh>
    }

    for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
    {
        std::shared_ptr<base::Node> childNode = std::make_shared<base::Node>(
                aiNode->mChildren[i]->mName.C_Str());
        processNode(childNode, aiNode->mChildren[i], scene, meshIndex, directory, texturesLoaded);
        node->children.push_back(childNode);
    }
}

std::shared_ptr<base::Mesh>
ModelManager::processMesh(aiMesh *mesh, const aiScene *scene, const std::string &meshName, const std::string &directory,
                          std::unordered_map<std::string, std::shared_ptr<base::Texture>> &texturesLoaded)
{
    std::vector<base::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<base::Texture>> textures;
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    // 处理顶点数据
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        base::Vertex vertex;
        glm::vec3 vector;

        // 处理位置
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // 处理法线
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        // 处理纹理坐标
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

    // 处理索引
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    // 处理纹理
    auto managerOpt = assets::AssetsMainManager::getManager(assets::AssetType::TEXTURE);
    if (managerOpt)
    {
        auto textureManagerPtr = std::dynamic_pointer_cast<assets::TextureManager>(managerOpt.value());
        if (textureManagerPtr)
        {
            for (aiTextureType aiType: supportedAiTextureTypes)
            {
                base::TextureType type = aiTextureTypeToTextureType(aiType);
                if (type == base::TextureType::Unknown) continue;

                for (unsigned int i = 0; i < material->GetTextureCount(aiType); i++)
                {
                    aiString path;
                    material->GetTexture(aiType, i, &path);
                    std::string texturePath = directory + "/" + path.C_Str();
                    std::vector<std::any> params = {texturePath, type};
                    auto textureUuid = textureManagerPtr->LoadResource(params);

                    if (textureUuid.has_value())
                    {
                        auto texture = textureManagerPtr->GetResourceByUuid(textureUuid.value());
                        if (texture.has_value())textures.push_back(texture.value());
                    }
                }
            }
        }
    }

    return std::make_shared<base::Mesh>(meshName, vertices, indices, textures);
}