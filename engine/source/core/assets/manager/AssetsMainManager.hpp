﻿//
// Created by Demon on 2023/12/12.
//

#ifndef DEMONENGINE_ASSETSMAINMANAGER_HPP
#define DEMONENGINE_ASSETSMAINMANAGER_HPP

//todo 处理模型，还包括纹理，材质、音频、着色器等其他资源的加载、缓存和卸载。
//todo 提供资源的加载，卸载，引用，缓存等

#include <unordered_map>
#include "core/base/interface/Interface.hpp"
#include "core/assets/AssetType.hpp"
#include "core/assets/interface/IFileManager.hpp"

namespace assets
{
    template<typename T>
    concept DerivedFromBase = std::is_base_of_v<IFileManager, T>;

    class AssetsMainManager
    {
    private:
        static std::unordered_map<AssetType, std::shared_ptr<IFileManager>> MANAGERS;
    public:
        static void initialize()
        {
            initializeManager();
        }

        static void finalize()
        {
            finalizeManager();
        }

        static std::optional<std::shared_ptr<IFileManager>> getManager(AssetType assetType);

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
#endif //DEMONENGINE_ASSETSMAINMANAGER_HPP
