//
// Created by Demon on 2024/3/27.
//

#include "Skybox.hpp"

namespace assets::scene
{
    Skybox::Skybox(const std::string &uuidStr, bool isUUID, std::shared_ptr<io::YamlConfiguration> &yml,
                   std::string name) : base::Object(uuidStr, isUUID),
                                       IMetaAccessor(yml, !isUUID, uuidStr.empty() ? nullptr : std::make_shared<base::UUID>(uuidStr, isUUID)),
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