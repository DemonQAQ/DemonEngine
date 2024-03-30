//
// Created by Demon on 2024/3/27.
//

#include "Skybox.hpp"

namespace assets::scene
{
    void Skybox::setName(const std::string &name_)
    {
        name = name_;
    }

    std::string Skybox::getName() const
    {
        return name;
    }

    Skybox::Skybox(std::string name) : base::Object(), name(std::move(name))
    {

    }

    std::string Skybox::Serialize() const
    {
        return std::string();
    }

    void Skybox::Deserialize(const std::string &data)
    {

    }
}