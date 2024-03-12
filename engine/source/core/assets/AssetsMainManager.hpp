//
// Created by Demon on 2023/12/12.
//

#ifndef DEMONENGINE_ASSETSMAINMANAGER_HPP
#define DEMONENGINE_ASSETSMAINMANAGER_HPP

//todo 处理模型，还包括纹理，材质、音频、着色器等其他资源的加载、缓存和卸载。
//todo 提供资源的加载，卸载，引用，缓存等

#include <unordered_map>
#include "core/base/interface/Interface.hpp"
#include "AssetType.hpp"
#include "core/assets/interface/IFileManager.hpp"
#include "TextureManager.hpp"

namespace assets
{
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

        }

        static std::optional<std::shared_ptr<IFileManager>> getManager(AssetType assetType)
        {
            auto it = MANAGERS.find(assetType);
            if (it != MANAGERS.end()) return it->second;
            else return std::nullopt;
        }

    private:
        static void initializeManager()
        {
            MANAGERS.insert({AssetType::TEXTURE, std::make_shared<TextureManager>()});
        }

        static void finalizeManager()
        {

        }
    };

    std::unordered_map<AssetType, std::shared_ptr<IFileManager>> AssetsMainManager::MANAGERS;
}
#endif //DEMONENGINE_ASSETSMAINMANAGER_HPP
