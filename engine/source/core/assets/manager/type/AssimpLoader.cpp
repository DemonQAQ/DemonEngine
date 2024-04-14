//
// Created by Demon on 2024/4/13.
//

#include <core/io/FileSystem.hpp>
#include <core/assets/manager/data/MaterialsManager.hpp>
#include <core/assets/AssetType.hpp>
#include <core/assets/manager/AssetsDataMainManager.hpp>
#include "AssimpLoader.hpp"
#include "ConfigLoader.hpp"

std::shared_ptr<base::Model> assets::AssimpLoader::loadModel(const std::string &path)
{
    std::string directory = path.substr(0, path.find_last_of('/'));
    std::string modelName = directory.substr(directory.find_last_of('/') + 1);
    directory = FileSystem::combinePaths(SOURCE_ROOT_PATH, directory);
    std::string metaYmlPath = directory + ".meta";

    auto metaYml = ConfigLoader::loadYml(metaYmlPath, true);
    if (!metaYml)return nullptr;

    bool init = metaYml->isEmpty();
    std::shared_ptr<base::UUID> existingUuid;
    std::string uuidStr;

    if (init)uuidStr = utils::uuidUtil::getUUID(directory);
    else uuidStr = metaYml->getString("uuid");

    existingUuid = base::UUIDManager::getUUID(uuidStr, false);

    return loadModel(directory, existingUuid, init, metaYml);
}

std::shared_ptr<base::Model>
assets::AssimpLoader::loadModel(const std::string &path, const std::shared_ptr<base::UUID> &existingUuid, bool init,
                                std::shared_ptr<io::YamlConfiguration> &yml)
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

void assets::AssimpLoader::processNode(const std::shared_ptr<base::Node> &node, aiNode *aiNode, const aiScene *scene,
                                       int &meshIndex, const std::string &directory, const Transform &parentTransform,
                                       const std::string &pathIdentifier)
{
    base::Transform nodeTransform = convertAiMatrixToTransform(aiNode->mTransformation);
    base::Transform globalTransform = base::Transform::merge({parentTransform, nodeTransform});

//        auto materialsManagerOpt = AssetsDataMainManager::getManager(AssetType::MATERIALS);
//    if (!materialsManagerOpt.has_value()) return nullptr;
//    auto materialsManager = std::dynamic_pointer_cast<MaterialsManager>(materialsManagerOpt.value());
//    if (!materialsManager) return;

    aiMaterial **material = scene->mMaterials;
    materialsManager->loadData({material, path});

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
assets::AssimpLoader::processMesh(aiMesh *mesh, const aiScene *scene, const std::string &meshName,
                                  const std::string &rootPath, const Transform &nodeTransform)
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

base::Transform assets::AssimpLoader::convertAiMatrixToTransform(const aiMatrix4x4 &aiMatrix)
{
    return base::Transform();
}

std::shared_ptr<base::Material>
MaterialsManager::loadMaterialFromAssimp(const aiMaterial *aiMat, const std::string &metadataPath)
{
    auto configManagerOpt = AssetsDataMainManager::getManager(AssetType::CONFIG);
    if (!configManagerOpt.has_value()) return nullptr;
    auto configManager = std::dynamic_pointer_cast<ConfigManager>(configManagerOpt.value());
    if (!configManager) return nullptr;

    auto metaUuid = configManager->loadData({metadataPath});
    if (!metaUuid.has_value())
    {
        std::cerr << "Failed to load metadata from: " << metadataPath << std::endl;
        return nullptr;
    }

    auto metaFileOpt = configManager->getResourceByUuid(metaUuid.value());
    if (!metaFileOpt.has_value())
    {
        std::cerr << "Failed to find metadata file with UUID: " << metaUuid.value()->toString() << std::endl;
        return nullptr;
    }

    auto metaFile = std::dynamic_pointer_cast<io::YamlConfiguration>(metaFileOpt.value());
    if (!metaFile)
    {
        std::cerr << "Metadata file is not a YAML configuration." << std::endl;
        return nullptr;
    }

    std::string uuidStr = metaFile->getString("uuid");
    bool init = uuidStr.empty();
    if (init) uuidStr = utils::uuidUtil::getUUID(metadataPath);
    auto materialUuid = UUIDManager::getUUID(uuidStr);
    auto it = loadedMaterial.find(materialUuid);
    if (it != loadedMaterial.end())
    {
        return it->second;
    }

    glm::vec3 diffuse(0.8f), specular(1.0f), ambient(0.2f), emissive(0.0f);
    float shininess = 32.0f, opacity = 1.0f, roughness = 0.5f, metallic = 0.0f, reflectivity = 0.5f;

    aiColor3D aiDiffuse(0.f, 0.f, 0.f);
    if (aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuse) == AI_SUCCESS)
    {
        diffuse = glm::vec3(aiDiffuse.r, aiDiffuse.g, aiDiffuse.b);
    }

    aiColor3D aiSpecular(0.f, 0.f, 0.f);
    if (aiMat->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecular) == AI_SUCCESS)
    {
        specular = glm::vec3(aiSpecular.r, aiSpecular.g, aiSpecular.b);
    }

    aiColor3D aiAmbient(0.f, 0.f, 0.f);
    if (aiMat->Get(AI_MATKEY_COLOR_AMBIENT, aiAmbient) == AI_SUCCESS)
    {
        ambient = glm::vec3(aiAmbient.r, aiAmbient.g, aiAmbient.b);
    }

    aiColor3D aiEmissive(0.f, 0.f, 0.f);
    if (aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, aiEmissive) == AI_SUCCESS)
    {
        emissive = glm::vec3(aiEmissive.r, aiEmissive.g, aiEmissive.b);
    }

    auto material = std::make_shared<base::Material>(materialUuid, init, metaFile, generateUniqueMaterialName(aiMat),
                                                     diffuse, specular, ambient, emissive,
                                                     shininess, opacity, roughness, metallic, reflectivity);
    return material;
}
