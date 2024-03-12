#include "Mesh.hpp"
#include "Model.hpp"

using namespace base;

Mesh::Mesh(std::string name, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
           const std::vector<std::shared_ptr<Texture>> &textures, const Transform &initialTransform)
        : name(std::move(name)), vertices(vertices), indices(indices), textures(textures)
{
    setTransform(initialTransform);
}

void Mesh::setName(const std::string &name_)
{
    name = name_;
}

std::string Mesh::getName() const
{
    return name;
}

void Mesh::updateTransformsBeforeRendering()
{
// Implement logic as needed
}

void Mesh::updateActualTransform(std::vector<Transform> &additionalTransforms)
{
// Implement logic for updating the actual transform
// This could involve combining additionalTransforms with the current transform
}

void Mesh::updateObservedActualTransform(std::vector<Transform> &additionalTransforms)
{
// Implement any additional logic as needed for observed transforms
}

RenderData Mesh::getRenderData(Transform combinedTransform)
{
// Implement logic for rendering, potentially using combinedTransform
// This function should return RenderData, which might include mesh data and other rendering parameters
    return RenderData{}; // Placeholder return, adapt as necessary
}

void Mesh::setFatherModel(const std::shared_ptr<Model> &model)
{
    fatherModel = model;
}

Transform Mesh::getLocalTransform() const
{
    return transform;
}