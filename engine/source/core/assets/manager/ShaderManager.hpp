//
// Created by Demon on 2023/12/12.
//

#ifndef DEMONENGINE_SHADERMANAGER_HPP
#define DEMONENGINE_SHADERMANAGER_HPP

#include <map>
#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileManager.hpp"
#include "core/base/render/Shader.hpp"

namespace assets
{
    class ShaderManager : implements IFileManager
    {
    private:
        std::map<base::UUID, std::shared_ptr<base::Shader>> shaders;
    public:
        std::optional<base::UUID> LoadResource(const std::vector<std::any> &params) override;

        void UnloadResource(const std::vector<std::any> &params) override;

        [[nodiscard]] bool IsResourceLoaded(const std::vector<std::any>& params) const override;

        void UpdateResource(const std::vector<std::any>& params) override;

        std::optional<std::shared_ptr<base::Shader>> GetResourceByUuid(const base::UUID &uuid);
    };
}

#endif //DEMONENGINE_SHADERMANAGER_HPP
