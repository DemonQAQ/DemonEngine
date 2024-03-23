//
// Created by Demon on 2024/3/21.
//
#include "Mesh.hpp"
#include "Model.hpp"
#include "RenderableObject.hpp"

#include <utility>

using namespace base;

RenderableObject::RenderableObject(std::string name, const std::vector<Model> &models,
                                   const base::Transform &initialTransform, UUID *shaderUUID, UUID *materialUUID)
        : name(std::move(name)), models(models)
{
    if (shaderUUID)bindShader(shaderUUID);
    else bindShader(getDefaultShader());

    if (materialUUID)bindMaterial(materialUUID);
    else bindMaterial(getDefaultMaterial());

    setTransform(initialTransform);
}

void RenderableObject::getRenderData(std::vector<RenderData> renderDataList)
{
    for (Model model: models)
    {
        model.getRenderData(renderDataList);
    }
}

void RenderableObject::beforeRendering(const std::vector<std::any> &params)
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

void RenderableObject::afterRendering(const std::vector<std::any> &params)
{
    updated = false;
}

void RenderableObject::updateGlobalTransform(std::vector<Transform> &additionalTransforms)
{
    if (!isTransformDirty() || additionalTransforms.empty())return;
    setTransformDirty(false);
    updateSelfGlobalTransform(additionalTransforms);
    updateObservedGlobalTransform(additionalTransforms);
}

void RenderableObject::updateObservedGlobalTransform(std::vector<Transform> &additionalTransforms)
{
    std::vector<Transform> transformsToMerge = {getLocalTransform()};
    transformsToMerge.insert(transformsToMerge.end(), additionalTransforms.begin(), additionalTransforms.end());
    for (Model &model: models)model.updateGlobalTransform(transformsToMerge);
}



