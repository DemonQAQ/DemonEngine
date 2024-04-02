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
        static std::map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Shader>> shaders;
    public:
        std::optional<std::shared_ptr<base::UUID>> loadResource(const std::vector<std::any> &params) override;

        void unloadResource(const std::vector<std::any> &params) override;

        [[nodiscard]] bool isResourceLoaded(const std::vector<std::any>& params) const override;

        void updateResource(const std::vector<std::any>& params) override;

        std::optional<std::shared_ptr<base::Shader>> getResourceByUuid(const std::shared_ptr<base::UUID>& uuid_ptr);
    };
}

#endif //DEMONENGINE_SHADERMANAGER_HPP
