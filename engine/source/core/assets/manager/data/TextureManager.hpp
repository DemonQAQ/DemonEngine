//
// Created by Demon on 2024/3/6.
//

#ifndef DEMONENGINE_TEXTUREMANAGER_HPP
#define DEMONENGINE_TEXTUREMANAGER_HPP

#include "core/base/lib/pch.hpp"
#include <unordered_map>
#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IDataManager.hpp"
#include "core/base/render/Texture.hpp"
#include "assimp/material.h"
#include "stb_image.h"

namespace assets
{
    class TextureManager : implements IDataManager
    {
    private:
        static std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Texture>> loadedTextures;

    public:
        TextureManager();

        /**
         * 创建并管理一个Texture实例
         *
         * @params[0] const std::shared_ptr<base::UUID> &existingUuid   实例的uuid
         * @params[1] bool init                                         是否第一次创建
         * @params[2] std::shared_ptr<io::YamlConfiguration> &yml       实例对应的meta文件
         * @params[3] TextureType type                                  贴图类型
         * @params[4] const std::string &path                           贴图路径
         * */
        bool loadData(const std::vector<std::any> &params) override;

        std::optional<std::shared_ptr<base::Texture>> getResourceByUuid(const std::shared_ptr<base::UUID>& uuid_ptr);

        void unloadData(const std::vector<std::any> &params) override;

        /**
         *
         * @params[0] const std::shared_ptr<base::UUID> &existingUuid   材质的uuid
         * */
        [[nodiscard]] bool isDataLoaded(const std::vector<std::any>& params) const override;

        void updateData(const std::vector<std::any>& params) override;

    private:
        static unsigned int loadTextureFromFile(const char *filePath);
    };
}

#endif // DEMONENGINE_TEXTUREMANAGER_HPP
