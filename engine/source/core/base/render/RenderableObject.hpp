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

namespace base
{
    class RenderableObject : implements Object, ITransformable, IRenderable
    {
    public:
        //todo 管理多个model
        //todo 管理多个shader的uuid
        //todo 管理map A，其中key使用shader的uuid，value使用一个二级指针的vector实例，其中每个值指向一个model/mesh的实例
        //todo 维护一个map B，key是model的uuid，value是一个vector，vector中储存绑定了shader的mesh
        //todo 渲染时通过A和B获得model和mesh关于shader的绑定信息
        std::vector<Model> models;
        std::vector<base::UUID> useShader;
        Transform transform;

        std::unordered_map<base::UUID, std::vector<IRenderable **>> shaderBindObjects;
        std::unordered_map<base::UUID, std::vector<std::weak_ptr<Mesh>>> bindShaderMeshList;

        RenderableObject(const std::vector<Model> &models = {}, const Transform &transform = Transform())
                : models(models), transform(transform)
        {
        }

        void render()
        {

        }

        RenderData getRenderData(Transform combinedTransform) override
        {

        }

        void updateActualTransform(const std::vector<Transform> &additionalTransforms) override
        {
            updateSelfActualTransform(additionalTransforms);
            //todo 更新所有model的transform
        }

        void updateObservedActualTransform(const std::vector<Transform> &additionalTransforms) const override
        {
            //todo 更新model的transform
        }

        void setPosition(const glm::vec3 &position) override
        {
            transform.position = position;
        }

        [[nodiscard]] glm::vec3 getPosition() const override
        {
            return transform.position;
        }

        void setRotation(const glm::quat &rotation) override
        {
            transform.rotation = rotation;
        }

        [[nodiscard]] glm::quat getRotation() const override
        {
            return transform.rotation;
        }

        void setScale(const glm::vec3 &scale) override
        {
            transform.scale = scale;
        }

        [[nodiscard]] glm::vec3 getScale() const override
        {
            return transform.scale;
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
