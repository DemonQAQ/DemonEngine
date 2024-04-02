#include "Mesh.hpp"
#include "Model.hpp"

using namespace base;

Mesh::Mesh(std::string name, const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
           const Transform &initialTransform, const std::shared_ptr<base::UUID> &shaderUUID,
           const std::shared_ptr<base::UUID> &materialUUID) : name(std::move(name)), vertices(vertices),
                                                              indices(indices)
{
    if (shaderUUID)bindShader(shaderUUID);
    else bindShader(getDefaultShader());

    if (materialUUID)bindMaterial(materialUUID);
    else bindMaterial(getDefaultMaterial());
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

void Mesh::beforeRendering(const std::vector<std::any> &params)
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

void Mesh::afterRendering(const std::vector<std::any> &params)
{
    updated = false;
}

void Mesh::updateGlobalTransform(std::vector<Transform> &additionalTransforms)
{
    if (!isTransformDirty() || additionalTransforms.empty())return;
    setTransformDirty(false);
    updateSelfGlobalTransform(additionalTransforms);
}

void Mesh::updateObservedGlobalTransform(std::vector<Transform> &additionalTransforms)
{
    return;
}

void Mesh::getRenderData(std::vector<RenderData> renderDataList)
{
    RenderData renderData;
    renderData.vertices = this->vertices;
    renderData.indices = this->indices;
    renderData.globalTransform = getGlobalTransform();
    renderData.useShader = this->getShader();
    renderData.material = this->getMaterial();
    renderDataList.push_back(renderData);
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
