//
// Created by Demon on 2024/3/27.
//

#include "Entity.hpp"

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
}