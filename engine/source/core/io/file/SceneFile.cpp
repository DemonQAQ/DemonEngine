//
// Created by Demon on 2024/3/29.
//

#include <core/base/utils/UUIDUtil.hpp>
#include <core/base/common/manager/UUIDManager.hpp>
#include "SceneFile.hpp"

using namespace io;

SceneFile::SceneFile(const std::string &path) : IFile(
        base::UUIDManager::getUUID(utils::uuidUtil::getReappearUUID(path), true), path)
{

}

//assets::scene::Scene SceneFile::loadScene()
//{
//    return nullptr;
//}

void SceneFile::saveScene(const assets::scene::Scene &scene, const std::string &path)
{

}

std::string SceneFile::serialize() const
{
    return std::string();
}

void SceneFile::deserialize(const std::string &data)
{

}

void SceneFile::load()
{

}

void SceneFile::save() const
{

}

bool SceneFile::isEmpty() const
{
    return false;
}
