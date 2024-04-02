//
// Created by Demon on 2024/3/27.
//

#include <runtime/base/RuntimeApplication.hpp>
#include "Entity.hpp"
#include "core/base/common/platform/Application.hpp"

namespace assets::scene
{

    void Entity::setName(const std::string &name_)
    {
        name = name_;
    }

    std::string Entity::getName() const
    {
        return name;
    }

    void Entity::beforeRendering(const std::vector<std::any> &params)
    {
        runtimeApp.getRenderManager()->submitEntity(renderableObject, render::RenderType::OPAQUE);
    }

    void Entity::afterRendering(const std::vector<std::any> &params)
    {

    }

    std::string Entity::Serialize() const
    {
        return std::string();
    }

    void Entity::Deserialize(const std::string &data)
    {

    }

    Entity::Entity(std::string name) : Object(), name(std::move(name))
    {
        renderableObject = std::make_shared<RenderableObject>("renderableObject");
    }

    void Entity::addModel(const std::shared_ptr<Model> &model)
    {
        renderableObject->addModel(model);
    }
}