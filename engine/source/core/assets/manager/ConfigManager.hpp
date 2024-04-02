//
// Created by Demon on 2024/3/7.
//

#ifndef DEMONENGINE_CONFIGMANAGER_HPP
#define DEMONENGINE_CONFIGMANAGER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileManager.hpp"
#include "core/io/interface/IFile.hpp"

namespace assets
{
    class ConfigManager : implements IFileManager
    {
    private:
        static std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<io::IFile>> loadedConfig;
    public:
        std::optional<std::shared_ptr<base::UUID>> loadResource(const std::vector<std::any>& params) override;

        void unloadResource(const std::vector<std::any>& params) override;

        [[nodiscard]] bool isResourceLoaded(const std::vector<std::any>& params) const override;

        void updateResource(const std::vector<std::any>& params) override;

        std::optional<std::shared_ptr<io::IFile>> getResourceByUuid(const std::shared_ptr<base::UUID>& uuid_ptr);
    };
}

#endif //DEMONENGINE_CONFIGMANAGER_HPP
