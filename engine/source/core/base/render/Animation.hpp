//
// Created by Demon on 2024/5/15.
//

#ifndef DEMONENGINE_ANIMATION_HPP
#define DEMONENGINE_ANIMATION_HPP

#include "AnimationBone.hpp"
#include <unordered_map>
#include <string>
#include <utility>

namespace base
{
    class Animation : implements INameable
    {
    private:
        std::string name;
        std::unordered_map<std::string, std::shared_ptr<AnimationBone>> bonesByName;
        std::unordered_map<int, std::shared_ptr<AnimationBone>> bonesById;
        float lastTimeStamp = -1;
        float speed = 1;
        bool upend = false;
        bool isFreeze = false;
    public:
        Animation(std::string name,
                  const std::unordered_map<std::string, std::shared_ptr<AnimationBone>> &bonesByName = {},
                  const std::unordered_map<int, std::shared_ptr<AnimationBone>> &bonesById = {}) :
                name(std::move(name)), bonesByName(bonesByName), bonesById(bonesById)
        {

        }

        Transform getBoneTransformById(int id)
        {
            auto it = bonesById.find(id);
            if (it != bonesById.end())
            {
                return it->second->getTransform();
            }
            return Transform();
        }

        Transform getBoneTransformByName(const std::string &name_)
        {
            auto it = bonesByName.find(name_);
            if (it != bonesByName.end())
            {
                return it->second->getTransform();
            }
            return Transform();
        }

        void start(float nowTimeStamp, bool reset = true)
        {
            if (!reset && isFreeze)
            {
                isFreeze = false;
                return;
            }
            lastTimeStamp = nowTimeStamp;
            if (reset)
            {
                for (auto &bone: bonesByName)
                {
                    bone.second->start(nowTimeStamp);
                }
            }
        }

        void updateAnimation(float deltaTime)
        {
            if (isFreeze) return;
            lastTimeStamp += deltaTime * speed * (upend ? -1.0f : 1.0f);
            for (auto &bone: bonesByName)
            {
                bone.second->updateAnimation(deltaTime);
            }
        }

        void freeze()
        {
            isFreeze = true;
        }

        void stop()
        {
            isFreeze = true;
            lastTimeStamp = 0;
            for (auto &bone: bonesByName)
            {
                bone.second->reset();
            }
        }

        void setUpend(bool upend_)
        {
            upend = upend_;
            for (auto &bone: bonesByName)
            {
                bone.second->setUpend(upend_);
            }
        }

        void setSpeed(float speed_)
        {
            speed = speed_;
            for (auto &bone: bonesByName)
            {
                bone.second->setSpeed(speed_);
            }
        }

        float getSpeed() const
        {
            return speed;
        }

        void setName(const std::string &name_) override
        {
            name = name_;
        }

        [[nodiscard]] std::string getName() const override
        {
            return name;
        }

        void addBone(std::shared_ptr<AnimationBone> &bone)
        {
            if (bone)
            {
                bonesByName[bone->getName()] = bone;
                bonesById[bone->getId()] = bone;
            }
        }

        bool removeBoneByName(const std::string &name_)
        {
            auto it = bonesByName.find(name_);
            if (it != bonesByName.end())
            {
                int id = it->second->getId();
                bonesById.erase(id);
                bonesByName.erase(it);
                return true;
            }
            return false;
        }

        bool removeBoneByName(int id)
        {
            auto it = bonesById.find(id);
            if (it != bonesById.end())
            {
                auto name_ = it->second->getName();
                bonesById.erase(id);
                bonesByName.erase(name_);
                return true;
            }
            return false;
        }

        std::shared_ptr<AnimationBone> getBoneByName(const std::string &name_)
        {
            auto it = bonesByName.find(name_);
            return it != bonesByName.end() ? it->second : nullptr;
        }

        std::shared_ptr<AnimationBone> getBoneById(int id)
        {
            auto it = bonesById.find(id);
            return it != bonesById.end() ? it->second : nullptr;
        }
    };
}

#endif //DEMONENGINE_ANIMATION_HPP
