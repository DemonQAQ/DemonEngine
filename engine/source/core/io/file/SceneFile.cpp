//
// Created by Demon on 2024/3/29.
//

#include "SceneFile.hpp"

using namespace io;

SceneFile::SceneFile(const std::string &path) : IFile(path)
{

}

assets::scene::Scene SceneFile::loadScene()
{
    return assets::scene::Scene();
}

void SceneFile::saveScene(const assets::scene::Scene &scene, const std::string &path)
{

}

std::string SceneFile::Serialize() const
{
    return std::string();
}

void SceneFile::Deserialize(const std::string &data)
{

}

void SceneFile::load(const std::string &path)
{

}

void SceneFile::save(const std::string &path) const
{

}
