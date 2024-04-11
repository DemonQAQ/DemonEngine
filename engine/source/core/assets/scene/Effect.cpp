//
// Created by Demon on 2024/3/27.
//

#include "Effect.hpp"

namespace assets::scene
{
    Effect::Effect(const std::string &uuidStr, bool isUUID, std::shared_ptr<io::YamlConfiguration> &yml,
                   std::string name) :
            Object(uuidStr, isUUID),
            IMetaAccessor(yml, !isUUID,uuidStr.empty() ? nullptr : std::make_shared<base::UUID>(uuidStr,isUUID)),
            name(std::move(name))
    {

    }

    void Effect::setName(const std::string &name_)
    {
        name = name_;
    }

    std::string Effect::getName() const
    {
        return name;
    }

    void Effect::beforeRendering(const std::vector<std::any> &params)
    {

    }

    void Effect::afterRendering(const std::vector<std::any> &params)
    {

    }

    std::string Effect::Serialize() const
    {
        return std::string();
    }

    void Effect::Deserialize(const std::string &data)
    {

    }
} // assets