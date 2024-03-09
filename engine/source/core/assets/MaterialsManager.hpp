﻿//
// Created by Demon on 2024/3/6.
//

#ifndef DEMONENGINE_MATERIALSMANAGER_HPP
#define DEMONENGINE_MATERIALSMANAGER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileManager.hpp"

namespace assets
{
    class MaterialsManager : implements IFileManager
    {
        std::optional<base::UUID> LoadResource(const std::vector<std::any>& params) override
        {

        }

        void UnloadResource(const std::vector<std::any>& params) override
        {

        }

        [[nodiscard]] bool IsResourceLoaded(const std::vector<std::any>& params) const override
        {

        }

        void UpdateResource(const std::vector<std::any>& params) override
        {

        }
    };
}

#endif //DEMONENGINE_MATERIALSMANAGER_HPP
