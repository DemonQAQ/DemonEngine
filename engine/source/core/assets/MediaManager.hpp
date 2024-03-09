﻿//
// Created by Demon on 2024/3/6.
//

#ifndef DEMONENGINE_MEDIAMANAGER_HPP
#define DEMONENGINE_MEDIAMANAGER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileManager.hpp"

namespace assets
{
    class MediaManager : implements IFileManager
    {
        base::UUID LoadResource(const std::string &path) override
        {

        }

        void UnloadResource(const std::string &path) override
        {

        }

        [[nodiscard]] bool IsResourceLoaded(const std::string &path) const override
        {

        }

        void UpdateResource(const std::string &path) override
        {

        }
    };
}

#endif //DEMONENGINE_MEDIAMANAGER_HPP
