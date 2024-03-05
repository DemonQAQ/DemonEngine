//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_RENDERABLEOBJECT_HPP
#define DEMONENGINE_RENDERABLEOBJECT_HPP


#include "Shader.hpp"
#include <glm/glm.hpp>
#include <collection.h>
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/ITransformable.hpp"
#include "Model.hpp"
#include "core/base/common/Transform.hpp"
#include "core/base/common/UUID.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"

namespace base
{
    class RenderableObject : implements Object, IRenderable, ITransformableUpdate, INameable
    {
    private:
        std::string name;
        std::vector<Model> models;
        std::vector<base::UUID> useShader;
        std::unordered_map<base::UUID, std::vector<IRenderable **>> shaderBindObjects;
        std::unordered_map<base::UUID, std::vector<std::weak_ptr<Mesh>>> bindShaderMeshList;

        void updateTransforms()
        {
            std::vector<Transform> emptyTransforms;
            updateActualTransform(emptyTransforms);
            updateObservedActualTransform(emptyTransforms);
        }

    public:
        RenderableObject(std::string name, const std::vector<Model> &models = {},
                         const base::Transform &initialTransform = base::Transform()) : name(std::move(name)),
                                                                                        models(models)
        {
            setTransform(initialTransform);
        }

        void render()
        {

        }

        RenderData getRenderData(Transform combinedTransform) override
        {

        }

        void setName(const std::string &name_) override
        {
            this->name = name_;
        }

        [[nodiscard]] std::string getName() const override
        {
            return name;
        }

        [[nodiscard]] Transform getLocalTransform() const override
        {
            return getTransform();
        }

        void updateTransformsBeforeRendering() override
        {
            if (!isTransformDirty())return;
            updateTransforms();
            setTransformDirty(false);
        }

        void updateActualTransform(std::vector<Transform> &additionalTransforms) override
        {
            if (additionalTransforms.empty())return;
            updateSelfActualTransform(additionalTransforms);
            updateObservedActualTransform(additionalTransforms);
        }

        void updateObservedActualTransform(std::vector<Transform> &additionalTransforms) override
        {
            std::vector<Transform> transformsToMerge = {getLocalTransform()};
            transformsToMerge.insert(transformsToMerge.end(), additionalTransforms.begin(), additionalTransforms.end());
            for (Model &model: models)model.updateActualTransform(transformsToMerge);
        }

        void bindShaderToMesh(const std::string &modelName, const std::string &meshName, std::unique_ptr<base::UUID> shader)
        {
            for (auto &model: models)
            {
                if (model.getName() == modelName)
                {
                    auto mesh = model.getMesh(meshName);
                    if (mesh)
                    {
                        bindShaderMeshList[*shader].push_back(mesh);
                        useShader.push_back(*shader);
                        return;
                    }
                }
            }
        }

        void unbindShaderFromMesh(const std::string &meshName)
        {
            for (auto &entry: bindShaderMeshList)
            {
                auto &meshList = entry.second;
                meshList.erase(std::remove_if(meshList.begin(), meshList.end(),
                                              [meshName](const std::weak_ptr<Mesh> &mesh)
                                              {
                                                  auto sharedMesh = mesh.lock();
                                                  if (sharedMesh && sharedMesh->getName() == meshName)return true;
                                                  else return false;
                                              }),
                               meshList.end());

                if (meshList.empty())
                {
                    useShader.erase(std::remove(useShader.begin(), useShader.end(), entry.first), useShader.end());
                }
            }
        }

        const UUID *getShaderForMesh(const std::string &meshName)
        {
            for (auto &entry: bindShaderMeshList)
            {
                for (auto &mesh: entry.second)
                {
                    auto sharedMesh = mesh.lock();
                    if (sharedMesh && sharedMesh->getName() == meshName)
                    {
                        return &entry.first;
                    }
                }
            }
            return nullptr;
        }

        std::vector<base::UUID> &getCurrentShader()
        {
            return useShader;
        }
    };
}

#endif //DEMONENGINE_RENDERABLEOBJECT_HPP
