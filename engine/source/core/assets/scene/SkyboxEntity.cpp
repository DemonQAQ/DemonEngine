//
// Created by Demon on 2024/3/27.
//

#include <runtime/base/RuntimeApplication.hpp>
#include "SkyboxEntity.hpp"

namespace assets::scene
{
    SkyboxEntity::SkyboxEntity(const std::shared_ptr<base::UUID> &existingUuid,
                               bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                               const std::shared_ptr<ISkyBox> &skybox, std::string name) :
            base::Object(existingUuid),
            IMetaAccessor(yml, init, existingUuid),
            name(std::move(name)), skybox(skybox)
    {

    }

    void SkyboxEntity::setName(const std::string &name_)
    {
        name = name_;
    }

    std::string SkyboxEntity::getName() const
    {
        return name;
    }

    std::string SkyboxEntity::serialize() const
    {
        return std::string();
    }

    void SkyboxEntity::deserialize(const std::string &data)
    {

    }

    SkyboxEntity::~SkyboxEntity()
    {
        skybox = nullptr;
    }

    std::shared_ptr<ISkyBox> &SkyboxEntity::getSkyBox()
    {
        return skybox;
    }

    void SkyboxEntity::beforeRendering(const std::vector<std::any> &params)
    {
        std::shared_ptr<base::IRenderable> iRenderable = skybox;
        runtimeApp.getRenderManager()->submitEntity(iRenderable, render::RenderType::OPAQUE);
    }

    void SkyboxEntity::afterRendering(const std::vector<std::any> &params)
    {

    }
}