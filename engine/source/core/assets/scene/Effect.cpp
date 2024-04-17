//
// Created by Demon on 2024/3/27.
//

#include "Effect.hpp"

namespace assets::scene
{
    Effect::Effect(const std::shared_ptr<base::UUID> &existingUuid,
                   bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                   std::string name) :
            base::Object(existingUuid),
            IMetaAccessor(yml, init, existingUuid),
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