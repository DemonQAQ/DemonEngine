//
// Created by Demon on 2024/3/27.
//

#include <runtime/base/RuntimeApplication.hpp>
#include "Entity.hpp"
#include "core/base/common/platform/Application.hpp"
#include <core/assets/manager/loader/ConfigLoader.hpp>
#include <core/assets/manager/loader/RenderObjectLoader.hpp>

namespace assets::scene
{
    Entity::Entity(const std::shared_ptr<base::UUID> &existingUuid,
                   bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                   std::string name) :
            base::Object(existingUuid),
            IMetaAccessor(yml, init, existingUuid),
            name(std::move(name))
    {
        //todo 尝试从yml中读取RenderObject的数据，没有则初始化
        std::shared_ptr<base::UUID> renderableObjectUuid = UUIDManager::getUUID(utils::uuidUtil::getUUID(), true);
        auto renderObjectMetaYml = assets::ConfigLoader::loadYml("/package0/scene/test.renderObject.meta", true);
        renderableObject = RenderObjectLoader::loadObject(renderableObjectUuid, true, "test",
                                                                renderObjectMetaYml);
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
        std::shared_ptr<base::IRenderable> iRenderable = renderableObject;
        runtimeApp.getRenderManager()->submitEntity(iRenderable, render::RenderType::TRANSPARENT);
    }

    void Entity::afterRendering(const std::vector<std::any> &params)
    {

    }

    std::string Entity::serialize() const
    {
        return std::string();
    }

    void Entity::deserialize(const std::string &data)
    {

    }


    void Entity::addModel(const std::shared_ptr<Model> &model)
    {
        renderableObject->addModel(model);
    }

    Entity::~Entity()
    {
        renderableObject = nullptr;
    }
}