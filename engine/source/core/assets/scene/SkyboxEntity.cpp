//
// Created by Demon on 2024/3/27.
//

#include "SkyboxEntity.hpp"

namespace assets::scene
{
    SkyboxEntity::SkyboxEntity(const std::shared_ptr<base::UUID> &existingUuid,
                   bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                   std::string name) :
            base::Object(existingUuid),
            IMetaAccessor(yml, init, existingUuid),
            name(std::move(name))
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

    std::string SkyboxEntity::Serialize() const
    {
        return std::string();
    }

    void SkyboxEntity::Deserialize(const std::string &data)
    {

    }
}