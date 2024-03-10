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
        std::optional<base::UUID> LoadResource(const std::vector<std::any>& params) override;

        void UnloadResource(const std::vector<std::any>& params) override;

        [[nodiscard]] bool IsResourceLoaded(const std::vector<std::any>& params) const override;

        void UpdateResource(const std::vector<std::any>& params) override;
    };
}

#endif //DEMONENGINE_CONFIGMANAGER_HPP
