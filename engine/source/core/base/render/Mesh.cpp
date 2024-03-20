#include "Mesh.hpp"
#include "Model.hpp"

using namespace base;

Mesh::Mesh(std::string name, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
           const std::shared_ptr<Material> &material, const Transform &initialTransform)
        : name(std::move(name)), vertices(vertices), indices(indices), material(material)
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

const std::vector<Vertex> &Mesh::getVertices() const
{
    return vertices;
}

void Mesh::setVertices(const std::vector<Vertex> &vertices_)
{
    Mesh::vertices = vertices_;
}

const std::vector<unsigned int> &Mesh::getIndices() const
{
    return indices;
}

void Mesh::setIndices(const std::vector<unsigned int> &indices_)
{
    Mesh::indices = indices_;
}

const std::shared_ptr<Material> &Mesh::getMaterial() const
{
    return material;
}

void Mesh::setMaterial(const std::shared_ptr<Material> &material_)
{
    Mesh::material = material_;
}
