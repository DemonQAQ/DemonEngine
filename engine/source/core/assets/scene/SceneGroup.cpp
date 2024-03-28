//
// Created by Demon on 2024/3/26.
//

#include "SceneGroup.hpp"

namespace assets::scene
{
    void SceneGroup::setName(const std::string &name_)
    {
        name = name_;
    }

    std::string SceneGroup::getName() const
    {
        return name;
    }

    void SceneGroup::beforeRendering(const std::vector<std::any> &params)
    {

    }

    void SceneGroup::afterRendering(const std::vector<std::any> &params)
    {

    }

    void SceneGroup::addChild(const std::shared_ptr<Object> &child)
    {
        auto uuid = child->getUUID();
        childrenMap[uuid] = child;
        childrenList.push_back(child);
    }

    bool SceneGroup::removeChild(const std::shared_ptr<Object> &child)
    {
        auto uuid = child->getUUID();
        if (childrenMap.erase(uuid))
        {
            auto it = std::find_if(childrenList.begin(), childrenList.end(),
                                   [&uuid](const std::shared_ptr<base::Object> &obj)
                                   { return obj->getUUID() == uuid; });
            if (it != childrenList.end())
            {
                childrenList.erase(it);
                return true;
            }
        }
        return false;
    }

    bool SceneGroup::removeChildByUUID(const UUID &uuid)
    {
        if (childrenMap.erase(uuid))
        {
            auto it = std::find_if(childrenList.begin(), childrenList.end(),
                                   [&uuid](const std::shared_ptr<base::Object> &obj)
                                   { return obj->getUUID() == uuid; });
            if (it != childrenList.end())
            {
                childrenList.erase(it);
                return true;
            }
        }
        return false;
    }

    bool SceneGroup::updateChildByUUID(const UUID &uuid, const std::shared_ptr<base::Object> &newChild)
    {
        auto it = childrenMap.find(uuid);
        if (it != childrenMap.end())
        {
            it->second = newChild;
            return true;
        }
        return false;
    }

    std::shared_ptr<base::Object> SceneGroup::findChildByUUID(const UUID &uuid)
    {
        auto it = childrenMap.find(uuid);
        if (it != childrenMap.end())
        {
            return it->second;
        }
        return nullptr;
    }

    const std::vector<std::shared_ptr<Object>> &SceneGroup::getChildren() const
    {
        return childrenList;
    }

    SceneGroup::SceneGroup(std::string name) : base::Object(), name(std::move(name))
    {

    }

}