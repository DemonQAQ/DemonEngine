//
// Created by Demon on 2024/3/7.
//

#ifndef DEMONENGINE_CONFIGMANAGER_HPP
#define DEMONENGINE_CONFIGMANAGER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileManager.hpp"

namespace assets
{
    class ConfigManager : implements IFileManager
    {
        void LoadResource(const std::string &path) override
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

#endif //DEMONENGINE_CONFIGMANAGER_HPP
