//
// Created by Demon on 2024/3/9.
//
#include "ModelManager.hpp"
#include "MaterialsManager.hpp"
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

    std::vector<std::any> paramsForCheck = {path};
    if (IsResourceLoaded(paramsForCheck))
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

bool ModelManager::IsResourceLoaded(const std::vector<std::any> &params) const
{
    // Implementation of IsResourceLoaded
    return false;
}

void ModelManager::UpdateResource(const std::vector<std::any> &params)
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

    processNode(rootNode, scene->mRootNode, scene, index, directory,
                convertAiMatrixToTransform(scene->mRootNode->mTransformation));

    auto model = std::make_shared<base::Model>(path, modelName, rootNode, base::Transform());
    return model ? std::optional<base::Model>(*model) : std::nullopt;
}

void ModelManager::processNode(const std::shared_ptr<base::Node> &node, aiNode *aiNode, const aiScene *scene,
                               int &meshIndex, const std::string &directory, const base::Transform &parentTransform)
{
    base::Transform nodeTransform = convertAiMatrixToTransform(aiNode->mTransformation);
    base::Transform globalTransform = base::Transform::merge({parentTransform, nodeTransform});

    for (unsigned int i = 0; i < aiNode->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[aiNode->mMeshes[i]];
        std::string meshName =
                mesh->mName.length > 0 ? mesh->mName.C_Str() : "mesh" + std::to_string(meshIndex++);

        auto newMesh = processMesh(mesh, scene, meshName, globalTransform);
        node->meshes.push_back(newMesh);
    }

    for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
    {
        std::shared_ptr<base::Node> childNode = std::make_shared<base::Node>(
                aiNode->mChildren[i]->mName.C_Str());
        processNode(childNode, aiNode->mChildren[i], scene, meshIndex, directory, globalTransform);
        node->children.push_back(childNode);
    }
}

std::shared_ptr<base::Mesh>
ModelManager::processMesh(aiMesh *mesh, const aiScene *scene, const std::string &meshName,const base::Transform &nodeTransform)
{
    std::vector<base::Vertex> vertices;
    std::vector<unsigned int> indices;

    // Process vertex data
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        base::Vertex vertex;
        glm::vec3 vector;

        // Position
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;

        // Normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }

        // Texture Coordinates
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

    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    std::shared_ptr<base::Material> material = nullptr;

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *aiMat = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<std::any> params = {aiMat};

        // 尝试从MaterialsManager获取材质
        auto materialsManagerOpt = AssetsMainManager::getManager(AssetType::MATERIALS);
        if (materialsManagerOpt)
        {
            auto materialsManagerPtr = std::dynamic_pointer_cast<assets::MaterialsManager>(materialsManagerOpt.value());
            auto materialUuid = materialsManagerPtr->LoadResource(params);
            if (materialUuid.has_value())
            {
                auto mat = materialsManagerPtr->GetResourceByUuid(materialUuid.value());
                if (mat.has_value())material = mat.value();
            }
        }
    }

    base::UUID materialUUID = material->getUuid();
    return std::make_shared<base::Mesh>(meshName, vertices, indices, nodeTransform, nullptr, &materialUUID);
}

base::Transform ModelManager::convertAiMatrixToTransform(const aiMatrix4x4 &aiMatrix)
{
    glm::mat4 matrix;
    matrix[0][0] = aiMatrix.a1;
    matrix[1][0] = aiMatrix.a2;
    matrix[2][0] = aiMatrix.a3;
    matrix[3][0] = aiMatrix.a4;
    matrix[0][1] = aiMatrix.b1;
    matrix[1][1] = aiMatrix.b2;
    matrix[2][1] = aiMatrix.b3;
    matrix[3][1] = aiMatrix.b4;
    matrix[0][2] = aiMatrix.c1;
    matrix[1][2] = aiMatrix.c2;
    matrix[2][2] = aiMatrix.c3;
    matrix[3][2] = aiMatrix.c4;
    matrix[0][3] = aiMatrix.d1;
    matrix[1][3] = aiMatrix.d2;
    matrix[2][3] = aiMatrix.d3;
    matrix[3][3] = aiMatrix.d4;

    base::Transform transform;
    transform.fromMatrix(matrix);
    return transform;
}

std::optional<std::shared_ptr<base::Model>> ModelManager::GetResourceByUuid(const base::UUID &uuid)
{
    return std::nullopt;
}