﻿//
// Created by Demon on 2023/12/12.
//

#ifndef DEMONENGINE_ASSETSDATAMAINMANAGER_HPP
#define DEMONENGINE_ASSETSDATAMAINMANAGER_HPP

#include <unordered_map>
#include "core/base/interface/Interface.hpp"
#include "core/assets/AssetType.hpp"
#include "core/assets/interface/IDataManager.hpp"

namespace assets
{
    template<typename T>
    concept DerivedFromBase = std::is_base_of_v<IDataManager, T>;

    class AssetsDataMainManager
    {
    private:
        static std::unordered_map<AssetType, std::shared_ptr<IDataManager>> MANAGERS;
    public:
        static void initialize()
        {
            initializeManager();
        }

        static void afterInitialize();

        static void onStart();

        static void finalize()
        {
            finalizeManager();
        }

        static void onStop();

        static std::optional<std::shared_ptr<IDataManager>> getManager(AssetType assetType);

        template<DerivedFromBase T>
        static std::shared_ptr<T> getManagerAs(AssetType assetType)
        {
            auto it = MANAGERS.find(assetType);
            if (it == MANAGERS.end()) return nullptr;

            auto castedManager = std::dynamic_pointer_cast<T>(it->second);
            if (!castedManager) return nullptr;

            return castedManager;
        }

        static std::string getManagerClassName(AssetType assetType);

    private:
        static void initializeManager();

        static void finalizeManager();
    };
}
#endif //DEMONENGINE_ASSETSDATAMAINMANAGER_HPP
