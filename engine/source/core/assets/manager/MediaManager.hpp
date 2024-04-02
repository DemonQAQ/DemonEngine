//
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
    public:
        std::optional<std::shared_ptr<base::UUID>> loadResource(const std::vector<std::any>& params) override;

        void unloadResource(const std::vector<std::any>& params) override;

        [[nodiscard]] bool isResourceLoaded(const std::vector<std::any>& params) const override;

        void updateResource(const std::vector<std::any>& params) override;
    };
}

#endif //DEMONENGINE_MEDIAMANAGER_HPP
