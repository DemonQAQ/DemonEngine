//
// Created by Demon on 2024/3/27.
//

#include "Effect.hpp"

namespace assets::scene
{
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

    Effect::Effect(std::string name) : base::Object(), name(std::move(name))
    {

    }
} // assets