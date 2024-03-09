//
// Created by Demon on 2024/3/9.
//
#include "Model.hpp"
#include "Mesh.hpp"
#include <assimp/Importer.hpp>

using namespace base;

Model::Model(const std::string &modelPath, const std::string &modelName,
             const std::unordered_map<std::string, std::shared_ptr<Texture>> &textures,
             const std::shared_ptr<Node> &root, const Transform &initialTransform)
        : Object(modelPath + modelName), name(modelName), rootNode(root), directory(modelPath), texturesLoaded(textures)
{
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

void Model::updateTransformsBeforeRendering()
{
    // Implementation as needed
}

void Model::updateActualTransform(std::vector<Transform> &additionalTransforms)
{
    updateSelfActualTransform(additionalTransforms);
    updateObservedActualTransform(additionalTransforms);
}

void Model::updateObservedActualTransform(std::vector<Transform> &additionalTransforms)
{
    // Combine transforms and update child nodes as necessary
}

RenderData Model::getRenderData(Transform combinedTransform)
{
    // Return rendering data for the model
    return RenderData{};
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
