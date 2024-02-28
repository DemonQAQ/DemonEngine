//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_RENDERABLEOBJECT_HPP
#define DEMONENGINE_RENDERABLEOBJECT_HPP


#include "Shader.hpp"
#include <glm/glm.hpp>
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/common/ITransformable.hpp"
#include "Model.hpp"
#include "core/base/common/Transform.hpp"

namespace base
{
    class RenderableObject : implements Object, implements ITransformable
    {
    public:
        std::unique_ptr<Model> model;
        std::unique_ptr<Shader> shader;
        Transform transform;
        // 存储mesh到shader的绑定关系，如果需要
        std::unordered_map<std::string, std::unique_ptr<Shader>> meshShaderBindings;

        RenderableObject(std::unique_ptr<Model> model, std::unique_ptr<Shader> shader)
                : model(std::move(model)), shader(std::move(shader))
        {}

        void render()
        {

        }

        // 实现ITransformable接口，使用Transform结构体
        void setPosition(const glm::vec3 &position) override
        {
            transform.position = position;
        }

        glm::vec3 getPosition() const override
        {
            return transform.position;
        }

        void setRotation(const glm::quat &rotation) override
        {
            transform.rotation = rotation;
        }

        glm::quat getRotation() const override
        {
            return transform.rotation;
        }

        void setScale(const glm::vec3 &scale) override
        {
            transform.scale = scale;
        }

        glm::vec3 getScale() const override
        {
            return transform.scale;
        }


        // Shader绑定管理方法
        void bindShaderToMesh(const std::string &meshName, std::unique_ptr<Shader> shader)
        {
            meshShaderBindings[meshName] = std::move(shader);
        }

        void unbindShaderFromMesh(const std::string &meshName)
        {
            meshShaderBindings.erase(meshName);
        }

        std::unique_ptr<Shader> *getShaderForMesh(const std::string &meshName)
        {
            auto it = meshShaderBindings.find(meshName);
            return it != meshShaderBindings.end() ? &it->second : nullptr;
        }

        std::unique_ptr<Shader> *getCurrentShader()
        {
            return &shader;
        }
    };
}

#endif //DEMONENGINE_RENDERABLEOBJECT_HPP
