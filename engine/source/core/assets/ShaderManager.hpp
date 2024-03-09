//
// Created by Demon on 2023/12/12.
//

#ifndef DEMONENGINE_SHADERMANAGER_HPP
#define DEMONENGINE_SHADERMANAGER_HPP

//todo 提供对shader的读取引用等基础操作

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileManager.hpp"

namespace assets
{
    class ShaderManager : implements IFileManager
    {
        std::optional<base::UUID> LoadResource(const std::vector<std::any> &params) override;

        void UnloadResource(const std::vector<std::any> &params) override;

        [[nodiscard]] bool IsResourceLoaded(const std::string &path) const override;

        void UpdateResource(const std::string &path) override;
    };
}

#endif //DEMONENGINE_SHADERMANAGER_HPP
