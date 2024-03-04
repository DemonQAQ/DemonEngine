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
    class RenderableObject : implements Object, IRenderable, ITransformableUpdate
    {
    private:
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
        //todo 管理多个model
        //todo 管理多个shader的uuid
        //todo 管理map A，其中key使用shader的uuid，value使用一个二级指针的vector实例，其中每个值指向一个model/mesh的实例
        //todo 维护一个map B，key是model的uuid，value是一个vector，vector中储存绑定了shader的mesh
        //todo 渲染时通过A和B获得model和mesh关于shader的绑定信息

        RenderableObject(const std::vector<Model> &models = {},
                         const base::Transform &initialTransform = base::Transform()) : models(models)
        {
            setTransform(initialTransform);
        }

        void render()
        {

        }

        RenderData getRenderData(Transform combinedTransform) override
        {

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

        // Shader绑定管理方法
        void bindShaderToMesh(const std::string &meshName, std::unique_ptr<Shader> shader)
        {

        }

        void unbindShaderFromMesh(const std::string &meshName)
        {

        }

        std::unique_ptr<Shader> *getShaderForMesh(const std::string &meshName)
        {

        }

        std::unique_ptr<Shader> *getCurrentShader()
        {

        }
    };
}

#endif //DEMONENGINE_RENDERABLEOBJECT_HPP
