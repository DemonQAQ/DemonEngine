//
// Created by Demon on 2024/4/13.
//

#include "assimp/scene.h"
#include <core/io/FileSystem.hpp>
#include <core/assets/manager/data/MaterialsManager.hpp>
#include <core/assets/AssetType.hpp>
#include <core/assets/manager/AssetsDataMainManager.hpp>
#include <core/assets/manager/data/ConfigManager.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "AssimpLoader.hpp"
#include "ConfigLoader.hpp"

using namespace base;

std::shared_ptr<base::Model> assets::AssimpLoader::loadModel(const std::string &path)
{
    std::string directory = path.substr(0, path.find_last_of('/'));
    std::string modelName = path.substr(path.find_last_of('/') + 1);
    directory = FileSystem::combinePaths(SOURCE_ROOT_PATH, directory);
    std::string metaYmlPath = path + ".mat.meta";

    auto metaYml = ConfigLoader::loadYml(metaYmlPath, true, false);
    if (!metaYml)return nullptr;

    bool init = metaYml->isEmpty();
    std::shared_ptr<base::UUID> existingUuid;
    std::string uuidStr;

    if (init)uuidStr = utils::uuidUtil::getUUID();
    else uuidStr = metaYml->getString("uuid");

    existingUuid = base::UUIDManager::getUUID(uuidStr, false);
    return loadModel(directory, modelName, existingUuid, init, metaYml);
}

std::shared_ptr<base::Model>
assets::AssimpLoader::loadModel(const std::string &directory, const std::string &modelName,
                                const std::shared_ptr<base::UUID> &existingUuid, bool init,
                                std::shared_ptr<io::YamlConfiguration> &yml)
{
    Assimp::Importer importer;
    std::string modelFullPath = directory + "/" + modelName;
    const aiScene *scene = importer.ReadFile(modelFullPath,
                                             aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return nullptr;
    }

    std::string metadataPath = modelFullPath + ".meta";
    auto metaYml = ConfigLoader::loadYml(metadataPath, true, true);
    if (!metaYml)return nullptr;

    std::vector<std::shared_ptr<base::Material>> materials = {};
    if (scene->HasMaterials())
    {
        for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
        {
            aiMaterial *aiMat = scene->mMaterials[i];
            auto material = loadMaterialFromAssimp(aiMat, metaYml, directory);
            if (material)
            {
                materials.push_back(material);
            }
        }
    }

    auto rootNode = std::make_shared<base::Node>(scene->mRootNode->mName.C_Str());
    int index = 0;

    std::string rootPathIdentifier = modelName + "." + scene->mRootNode->mName.C_Str();

    processNode(rootNode, scene->mRootNode, scene, index, directory,
                convertAiMatrixToTransform(scene->mRootNode->mTransformation), rootPathIdentifier, materials);

    auto model = std::make_shared<base::Model>(existingUuid, init, modelName, rootNode, yml);
    model->bindMeshesToModel(model->getRootNode());
    return model;
}

void assets::AssimpLoader::processNode(const std::shared_ptr<base::Node> &node, aiNode *aiNode, const aiScene *scene,
                                       int &meshIndex, const std::string &directory, const Transform &parentTransform,
                                       const std::string &pathIdentifier,
                                       std::vector<std::shared_ptr<base::Material>> &materials)
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

        if (!materials.empty() && mesh->mMaterialIndex < materials.size())
        {
            std::shared_ptr<base::Material> firstMaterial = materials[mesh->mMaterialIndex];
            newMesh->bindMaterial(firstMaterial->getUUID());
        }

        node->meshes.push_back(newMesh);
    }

    for (unsigned int i = 0; i < aiNode->mNumChildren; i++)
    {
        std::shared_ptr<base::Node> childNode = std::make_shared<base::Node>(aiNode->mChildren[i]->mName.C_Str());
        std::string childPathIdentifier = pathIdentifier + "." + aiNode->mChildren[i]->mName.C_Str();

        processNode(childNode, aiNode->mChildren[i], scene, meshIndex, directory, globalTransform, childPathIdentifier,
                    materials);
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

    return std::make_shared<base::Mesh>(meshUuid, meshName, vertices, indices, nodeTransform);
}

base::Transform assets::AssimpLoader::convertAiMatrixToTransform(const aiMatrix4x4 &aiMatrix)
{
    glm::mat4 matrix = glm::transpose(glm::make_mat4(&aiMatrix.a1));
    glm::vec3 translation(matrix[3]);

    glm::vec3 scale;
    scale.x = glm::length(glm::vec3(matrix[0]));
    scale.y = glm::length(glm::vec3(matrix[1]));
    scale.z = glm::length(glm::vec3(matrix[2]));

    glm::mat3 rotationMatrix;
    rotationMatrix[0] = glm::normalize(glm::vec3(matrix[0]));
    rotationMatrix[1] = glm::normalize(glm::vec3(matrix[1]));
    rotationMatrix[2] = glm::normalize(glm::vec3(matrix[2]));

    glm::quat rotation = glm::quat_cast(rotationMatrix);
    return base::Transform(translation, rotation, scale);
}

std::shared_ptr<base::Material>
assets::AssimpLoader::loadMaterialFromAssimp(const aiMaterial *aiMat, std::shared_ptr<io::YamlConfiguration> &metaYml,
                                             const std::string &directory)
{
    auto materialsManagerOpt = AssetsDataMainManager::getManager(AssetType::MATERIALS);
    if (!materialsManagerOpt.has_value()) return nullptr;
    auto materialsManager = std::dynamic_pointer_cast<MaterialsManager>(materialsManagerOpt.value());
    if (!materialsManager) return nullptr;

    aiString matName;
    std::string materialName;
    if (aiMat->Get(AI_MATKEY_NAME, matName) == AI_SUCCESS)
    {
        materialName = matName.C_Str();
    }

    std::string uuidStr = metaYml->getString("materials." + materialName + ".uuid");
    bool init = uuidStr.empty();
    if (init) uuidStr = utils::uuidUtil::getUUID();
    auto materialUuid = UUIDManager::getUUID(uuidStr, false);
    if (materialsManager->isDataLoaded({materialUuid}))
    {
        auto materialOpt = materialsManager->getResourceByUuid(materialUuid);
        if (materialOpt.has_value())
        {
            auto material = std::dynamic_pointer_cast<base::Material>(materialOpt.value());
            return material;
        }
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

    std::map<TextureType, std::map<std::shared_ptr<base::UUID>, std::shared_ptr<Texture>>> textures;
    for (int type = aiTextureType_NONE + 1; type < aiTextureType_UNKNOWN; ++type)
    {
        aiString texPath;
        unsigned int textureCount = aiMat->GetTextureCount((aiTextureType) type);

        for (unsigned int i = 0; i < textureCount; ++i)
        {
            if (AI_SUCCESS == aiMat->GetTexture((aiTextureType) type, i, &texPath))
            {
                std::string fullPath = directory + "/" + texPath.C_Str();
                auto texture = loadTextureFromAssimp((aiTextureType) type, fullPath);

                if (texture)
                {
                    TextureType genericType = aiTextureTypeToTextureType((aiTextureType) type);
                    if (textures.find(genericType) == textures.end())
                    {
                        textures[genericType] = std::map<std::shared_ptr<base::UUID>, std::shared_ptr<Texture>>();
                    }
                    textures[genericType][texture->getUUID()] = texture;
                }
            }
        }
    }

    if (materialsManager->loadData(
            {materialUuid, init, metaYml, materialName, diffuse, specular, ambient, emissive, shininess, opacity,
             roughness, metallic, reflectivity, textures}))
    {
        auto materialOpt = materialsManager->getResourceByUuid(materialUuid);
        if (!materialOpt.has_value()) return nullptr;

        auto material = std::dynamic_pointer_cast<base::Material>(materialOpt.value());
        return material;
    }
    else return nullptr;
}

std::shared_ptr<base::Texture>
assets::AssimpLoader::loadTextureFromAssimp(const aiTextureType &aiType, const std::string &texturePath)
{
    base::TextureType textureType = aiTextureTypeToTextureType(aiType);
    std::string metadataPath = texturePath + ".meta";
    auto metaYml = ConfigLoader::loadYml(metadataPath, true, true);
    if (!metaYml)return nullptr;

    bool init = metaYml->isEmpty();
    std::shared_ptr<base::UUID> existingUuid;
    std::string uuidStr;

    if (init)uuidStr = utils::uuidUtil::getUUID();
    else uuidStr = metaYml->getString("uuid");

    existingUuid = base::UUIDManager::getUUID(uuidStr, false);

    auto texturesManagerOpt = AssetsDataMainManager::getManager(AssetType::TEXTURE);
    if (!texturesManagerOpt.has_value()) return nullptr;
    auto texturesManager = std::dynamic_pointer_cast<TextureManager>(texturesManagerOpt.value());
    if (!texturesManager) return nullptr;

    bool isLoad = texturesManager->isDataLoaded({existingUuid});

    if (isLoad || texturesManager->loadData({existingUuid, init, metaYml, textureType, texturePath}))
    {
        auto textureOpt = texturesManager->getResourceByUuid(existingUuid);
        if (!textureOpt.has_value()) return nullptr;

        auto texture = std::dynamic_pointer_cast<base::Texture>(textureOpt.value());
        return texture;
    }
    else return nullptr;
}
