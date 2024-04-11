//
// Created by Demon on 2024/3/9.
//
#include "ModelManager.hpp"
#include "MaterialsManager.hpp"
#include "assimp/Importer.hpp"
#include "ConfigManager.hpp"
#include <iostream>
#include <core/io/FileSystem.hpp>

using namespace assets;

std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Model>> ModelManager::loadedModels;

ModelManager::ModelManager()
{
}

/**
 * @params[0] std::string 模型相对资源文件架的路径
 * */
std::optional<std::shared_ptr<base::UUID>> ModelManager::loadResource(const std::vector<std::any> &params)
{
    if (params.empty() || params[0].type() != typeid(std::string))
    {
        std::cerr << "Invalid parameters for loadResource." << std::endl;
        return std::nullopt;
    }

    std::string sourcePath;
    std::string metadataPath;
    try
    {
        sourcePath = std::any_cast<std::string>(params[0]);
        sourcePath = FileSystem::combinePaths(SOURCE_ROOT_PATH, sourcePath);
        metadataPath = sourcePath + ".meta";
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error extracting parameters: " << e.what() << std::endl;
        return std::nullopt;
    }

    auto configManagerOpt = AssetsMainManager::getManager(AssetType::CONFIG);
    if (!configManagerOpt.has_value()) return nullptr;
    auto configManager = std::dynamic_pointer_cast<ConfigManager>(configManagerOpt.value());
    if (!configManager) return nullptr;

    auto metaUuid = configManager->loadResource({metadataPath});
    if (!metaUuid.has_value())
    {
        std::cerr << "Failed to load metadata from: " << metadataPath << std::endl;
        return std::nullopt;
    }

    auto metaFileOpt = configManager->getResourceByUuid(metaUuid.value());
    if (!metaFileOpt.has_value())
    {
        std::cerr << "Failed to find metadata file with UUID: " << metaUuid.value()->toString() << std::endl;
        return std::nullopt;
    }

    auto metaFile = std::dynamic_pointer_cast<io::YamlConfiguration>(metaFileOpt.value());
    if (!metaFile)
    {
        std::cerr << "Metadata file is not a YAML configuration." << std::endl;
        return std::nullopt;
    }

    std::string uuidStr = metaFile->getString("uuid");
    bool init = uuidStr.empty();
    if (init) uuidStr = utils::uuidUtil::getUUID(sourcePath);
    auto modelUuid = UUIDManager::getUUID(uuidStr);
    auto it = loadedModels.find(modelUuid);
    if (it != loadedModels.end())
    {
        return it->first;
    }

    auto model = loadModel(sourcePath, modelUuid, init, metaFile);
    if (!model)
    {
        return std::nullopt;
    }
    loadedModels[modelUuid] = model;
    return modelUuid;
}

void ModelManager::unloadResource(const std::vector<std::any> &params)
{
    // Implementation of unloadResource
}

bool ModelManager::isResourceLoaded(const std::vector<std::any> &params) const
{
    // Implementation of isResourceLoaded
    return false;
}

void ModelManager::updateResource(const std::vector<std::any> &params)
{
    // Implementation of updateResource
}

std::shared_ptr<base::Model>
ModelManager::loadModel(const std::string &path, const std::shared_ptr<base::UUID> &existingUuid,
                        bool init, std::shared_ptr<io::YamlConfiguration> &yml)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                                   aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    std::string directory = path.substr(0, path.find_last_of('/'));
    std::string modelName = directory.substr(directory.find_last_of('/') + 1);
    auto rootNode = std::make_shared<base::Node>(scene->mRootNode->mName.C_Str());
    int index = 0;

    std::string rootPathIdentifier = path + "." + scene->mRootNode->mName.C_Str();

    processNode(rootNode, scene->mRootNode, scene, index, directory,
                convertAiMatrixToTransform(scene->mRootNode->mTransformation), rootPathIdentifier);

    auto model = std::make_shared<base::Model>(existingUuid, init, modelName, rootNode, yml);
    return model;
}

void ModelManager::processNode(const std::shared_ptr<base::Node> &node, aiNode *aiNode, const aiScene *scene,
                               int &meshIndex, const std::string &directory, const base::Transform &parentTransform,
                               const std::string &pathIdentifier)
{
    base::Transform nodeTransform = convertAiMatrixToTransform(aiNode->mTransformation);
    base::Transform globalTransform = base::Transform::merge({parentTransform, nodeTransform});

    for (unsigned int i = 0; i < aiNode->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[aiNode->mMeshes[i]];
        std::string meshName =
                mesh->mName.length > 0 ? mesh->mName.C_Str() : "mesh" + std::to_string(meshIndex++);

        std::string meshPathIdentifier = pathIdentifier;
        meshPathIdentifier.append(".");
        meshPathIdentifier.append(meshName);

        auto newMesh = processMesh(mesh, scene, meshName, meshPathIdentifier, globalTransform);
        node->meshes.push_back(newMesh);
    }

    for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
    {
        std::shared_ptr<base::Node> childNode = std::make_shared<base::Node>(aiNode->mChildren[i]->mName.C_Str());
        std::string childPathIdentifier = pathIdentifier + "." + aiNode->mChildren[i]->mName.C_Str();

        processNode(childNode, aiNode->mChildren[i], scene, meshIndex, directory, globalTransform, childPathIdentifier);
        node->children.push_back(childNode);
    }
}

std::shared_ptr<base::Mesh>
ModelManager::processMesh(aiMesh *mesh, const aiScene *scene,
                          const std::string &meshName,
                          const std::string &rootPath,
                          const base::Transform &nodeTransform)
{
    std::string uuidStr = utils::uuidUtil::getReappearUUID(rootPath);
    auto meshUuid = UUIDManager::getUUID(uuidStr);

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
        }
        else
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

    return std::make_shared<base::Mesh>(meshUuid, meshName, vertices, indices, nodeTransform, nullptr, nullptr);
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

std::optional<std::shared_ptr<base::Model>> ModelManager::getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto it = loadedModels.find(uuid_ptr);
    if (it != loadedModels.end())return it->second;
    else return std::nullopt;
}

void ModelManager::updateModelRelation(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto model = getResourceByUuid(uuid_ptr);
    if (!model.has_value())return;

    auto metadata = model.value()->getMetadata();
    auto materialsUUIDMap = std::any_cast<std::map<std::shared_ptr<UUID>, std::shared_ptr<UUID>>>(
            metadata->getValue("materials"));

    auto selfMaterialUuid = materialsUUIDMap[uuid_ptr];
    if (selfMaterialUuid) model.value()->bindMaterial(selfMaterialUuid);

    std::function<void(const std::shared_ptr<Node> &)> updateMeshMaterial;
    updateMeshMaterial = [&materialsUUIDMap, &updateMeshMaterial, &selfMaterialUuid](const std::shared_ptr<Node> &node)
    {
        for (const auto &mesh: node->meshes)
        {
            auto it = materialsUUIDMap.find(mesh->getUUID());
            if (it != materialsUUIDMap.end())mesh->bindMaterial(it->second);
            else mesh->bindMaterial(selfMaterialUuid);
        }
        for (const auto &child: node->children)
        {
            updateMeshMaterial(child);
        }
    };
    updateMeshMaterial(model.value()->getRootNode());
}
