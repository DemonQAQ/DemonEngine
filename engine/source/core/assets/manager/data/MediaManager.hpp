//
// Created by Demon on 2024/3/6.
//

#ifndef DEMONENGINE_MEDIAMANAGER_HPP
#define DEMONENGINE_MEDIAMANAGER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IDataManager.hpp"

namespace assets
{
    class MediaManager : implements IDataManager
    {
    public:
        bool loadData(const std::vector<std::any>& params) override;

        void unloadData(const std::vector<std::any>& params) override;

        [[nodiscard]] bool isDataLoaded(const std::vector<std::any>& params) const override;

        void updateData(const std::vector<std::any>& params) override;

        void onStart() override;

        void onStop() override;
    };
}

#endif //DEMONENGINE_MEDIAMANAGER_HPP
