﻿//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_RENDERABLEOBJECT_HPP
#define DEMONENGINE_RENDERABLEOBJECT_HPP


#include "Shader.hpp"
#include <glm/glm.hpp>
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/ITransformable.hpp"
#include "Model.hpp"
#include "core/base/common/Transform.hpp"

namespace base
{
    class RenderableObject : implements Object, implements ITransformable
    {
    public:
        //todo 管理多个model
        //todo 管理多个shader的uuid
        //todo 管理map A，其中key使用shader的uuid，value使用一个二级指针的vector实例，其中每个值指向一个model/mesh的实例
        //todo 考虑为model和mesh实现统一接口用于在渲染时传递所需数据
        //todo 维护一个map B，key是model的uuid，value是一个vector，vector中储存绑定了shader的mesh
        //todo 渲染时通过A和B获得model和mesh关于shader的绑定信息
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
