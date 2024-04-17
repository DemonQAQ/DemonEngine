//
// Created by Demon on 2024/3/27.
//

#include "Skybox.hpp"

namespace assets::scene
{
    Skybox::Skybox(const std::shared_ptr<base::UUID> &existingUuid,
                   bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                   std::string name) :
            base::Object(existingUuid),
            IMetaAccessor(yml, init, existingUuid),
            name(std::move(name))
    {

    }

    void Skybox::setName(const std::string &name_)
    {
        name = name_;
    }

    std::string Skybox::getName() const
    {
        return name;
    }

    std::string Skybox::Serialize() const
    {
        return std::string();
    }

    void Skybox::Deserialize(const std::string &data)
    {

    }
}