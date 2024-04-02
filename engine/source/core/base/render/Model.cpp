//
// Created by Demon on 2024/3/9.
//

#include "Mesh.hpp"
#include "Texture.hpp"
#include <assimp/Importer.hpp>
#include "Model.hpp"

using namespace base;

Model::Model(const std::string &modelPath, const std::string &modelName,
             const std::shared_ptr<Node> &root, const Transform &initialTransform,
             const std::shared_ptr<base::UUID> &shaderUUID, const std::shared_ptr<base::UUID> &materialUUID)
        : Object(modelPath + modelName), name(modelName), rootNode(root), directory(modelPath)
{
    if (shaderUUID)bindShader(shaderUUID);
    else bindShader(getDefaultShader());

    if (materialUUID)bindMaterial(materialUUID);
    else bindMaterial(getDefaultMaterial());
    setTransform(initialTransform);
    // Initialize bonesInfo, boneCount, etc. here if necessary
    bindMeshesToModel(rootNode);
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

void Model::getRenderData(std::vector<RenderData> renderDataList)
{
    if (!rootNode)return;
    processNode(rootNode, renderDataList);
}

Transform Model::getLocalTransform() const
{
    return getTransform();
}

std::shared_ptr<Mesh> Model::getMesh(const std::string &meshName)
{
    // Implementation for finding a mesh by name within the model
    return nullptr; // Placeholder return
}

void Model::bindMeshesToModel(const std::shared_ptr<Node> &node)
{
    // Bind meshes to model, setting fatherModel for each mesh
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
