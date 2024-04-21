//
// Created by Demon on 2024/3/9.
//

#include "Mesh.hpp"
#include "Texture.hpp"
#include <assimp/Importer.hpp>
#include <utility>
#include "Model.hpp"

using namespace base;

Model::Model(const std::shared_ptr<base::UUID> &existingUuid,
             bool init,
             std::string modelName,
             const std::shared_ptr<Node> &root,
             std::shared_ptr<io::YamlConfiguration> &yml,
             const Transform &initialTransform,
             const std::shared_ptr<base::UUID> &shaderUUID,
             const std::shared_ptr<base::UUID> &materialUUID)
        : Object(existingUuid),
          IMetaAccessor(yml, init, existingUuid),
          name(std::move(modelName)), rootNode(root)
{
    if (shaderUUID)bindShader(shaderUUID);
    else bindShader(getDefaultShader());

    if (materialUUID)bindMaterial(materialUUID);
    else bindMaterial(getDefaultMaterial());
    setTransform(initialTransform);
    // Initialize bonesInfo, boneCount, etc. here if necessary
}

void Model::setName(const std::string &name_)
{
    name = name_;
}

std::string Model::getName() const
{
    return name;
}

void Model::beforeRendering(const std::vector<std::any> &params)
{
    if (updated)return;
    std::vector<Transform> additionalTransforms;
    if (!params.empty())
    {
        if (params[0].type() == typeid(const std::vector<Transform>))
            additionalTransforms = std::any_cast<std::vector<Transform>>(params[0]);
        updateGlobalTransform(additionalTransforms);
    }

    updated = true;
}

void Model::afterRendering(const std::vector<std::any> &params)
{
    updated = false;
}

void Model::updateGlobalTransform(std::vector<Transform> &additionalTransforms)
{
    if (!isTransformDirty() || additionalTransforms.empty())return;
    setTransformDirty(false);
    updateSelfGlobalTransform(additionalTransforms);
    updateObservedGlobalTransform(additionalTransforms);
}

void Model::updateObservedGlobalTransform(std::vector<Transform> &additionalTransforms)
{
    std::vector<Transform> transformsToMerge = {getLocalTransform()};
    transformsToMerge.insert(transformsToMerge.end(), additionalTransforms.begin(), additionalTransforms.end());

    std::function<void(const std::shared_ptr<Node> &, const std::vector<Transform> &)> updateNodeTransforms;
    updateNodeTransforms = [&](const std::shared_ptr<Node> &node, const std::vector<Transform> &parentTransforms)
    {
        for (const auto &mesh: node->meshes)mesh->updateGlobalTransform(transformsToMerge);
        for (const auto &child: node->children)updateNodeTransforms(child, transformsToMerge);
    };

    if (rootNode)updateNodeTransforms(rootNode, transformsToMerge);
}

void Model::getRenderData(std::vector<RenderData> &renderDataList)
{
    if (!rootNode)return;
    processNode(rootNode, renderDataList);
}

Transform Model::getLocalTransform() const
{
    return getTransform();
}

std::shared_ptr<Mesh> Model::getMesh(const std::shared_ptr<base::UUID> &uuidPtr)
{
    std::function<std::shared_ptr<Mesh>(const std::shared_ptr<Node> &)> searchMesh;
    searchMesh = [&searchMesh, &uuidPtr](const std::shared_ptr<Node> &node) -> std::shared_ptr<Mesh>
    {
        for (const auto &mesh: node->meshes)
        {
            if (mesh->getUUID() == uuidPtr)
            {
                return mesh;
            }
        }
        for (const auto &child: node->children)
        {
            auto foundMesh = searchMesh(child);
            if (foundMesh != nullptr)
            {
                return foundMesh;
            }
        }
        return nullptr;
    };

    return searchMesh(rootNode);
}

void Model::bindMeshesToModel(const std::shared_ptr<Node> &node)
{
    std::function<void(const std::shared_ptr<Node> &)> bindMesh;
    bindMesh = [this, &bindMesh](const std::shared_ptr<Node> &node)
    {
        for (const auto &mesh: node->meshes)
        {
            mesh->setFatherModel(shared_from_this());
        }
        for (const auto &child: node->children)
        {
            bindMesh(child);
        }
    };
    bindMesh(node);
}

void Model::processNode(const std::shared_ptr<Node> &node, std::vector<RenderData> &renderDataList)
{
    for (auto &mesh: node->meshes)
    {
        mesh->getRenderData(renderDataList);
    }

    for (auto &child: node->children)
    {
        processNode(child, renderDataList);
    }
}

std::shared_ptr<Node> &Model::getRootNode()
{
    return rootNode;
}

void Model::updateAllMeshShader()
{
// Check if there's a default shader assigned at the model level
    auto defaultShaderUUID = this->getShader();
    if (!defaultShaderUUID)
    {
        std::cerr << "No default shader UUID provided to Model." << std::endl;
        return;
    }

    // Function to recursively traverse nodes and update mesh shaders
    std::function<void(const std::shared_ptr<Node> &)> updateNodeShaders = [&](const std::shared_ptr<Node> &node)
    {
        if (!node) return;

        for (auto &mesh: node->meshes)
        {
            // If the mesh does not have a shader, assign the default shader
            if (!mesh->getShader())
            {
                mesh->bindShader(defaultShaderUUID);
            }
        }

        // Recurse for each child node
        for (auto &child: node->children)
        {
            updateNodeShaders(child);
        }
    };

    // Start the shader update from the root node
    updateNodeShaders(rootNode);
}
