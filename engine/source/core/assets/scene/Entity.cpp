//
// Created by Demon on 2024/3/27.
//

#include <runtime/base/RuntimeApplication.hpp>
#include "Entity.hpp"
#include "core/base/common/platform/Application.hpp"

namespace assets::scene
{
    Entity::Entity(const std::string &uuidStr, bool isUUID, std::shared_ptr<io::YamlConfiguration> &yml,
                   std::string name) : Object(uuidStr, isUUID),
                                       IMetaAccessor(yml, !isUUID, uuidStr.empty() ? nullptr : std::make_shared<base::UUID>(uuidStr, isUUID)),
                   name(std::move(name))
    {
        renderableObject = std::make_shared<RenderableObject>("renderableObject");
    }

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


    void Entity::addModel(const std::shared_ptr<Model> &model)
    {
        renderableObject->addModel(model);
    }
}