//
// Created by Demon on 2024/3/6.
//

#ifndef DEMONENGINE_TEXTUREMANAGER_HPP
#define DEMONENGINE_TEXTUREMANAGER_HPP

#include "core/base/lib/pch.hpp"
#include <unordered_map>
#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileManager.hpp"
#include "core/base/render/Texture.hpp"
#include "assimp/material.h"
#include "stb_image.h"

namespace assets
{
    static base::TextureType aiTextureTypeToTextureType(aiTextureType aiType)
    {
        switch (aiType)
        {
            case aiTextureType_DIFFUSE:
                return base::TextureType::DIFFUSE;
            case aiTextureType_SPECULAR:
                return base::TextureType::SPECULAR;
            case aiTextureType_HEIGHT:
                return base::TextureType::HEIGHT;
            case aiTextureType_AMBIENT:
                return base::TextureType::AMBIENT_OCCLUSION;
            case aiTextureType_EMISSIVE:
                return base::TextureType::EMISSIVE;
            case aiTextureType_NORMALS:
                return base::TextureType::NORMAL;
            default:
                return base::TextureType::UNKNOWN;
        }
    }

    static const std::vector<aiTextureType> supportedAiTextureTypes = {
            aiTextureType_DIFFUSE,
            aiTextureType_SPECULAR,
            aiTextureType_HEIGHT,
            aiTextureType_AMBIENT,
            aiTextureType_EMISSIVE,
            aiTextureType_NORMALS,
    };

    class TextureManager : implements IFileManager
    {
    private:
        static std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Texture>> loadedTextures;

    public:
        //params1 = path(string), params2 = textureType(base::TextureType)
        std::optional<std::shared_ptr<base::UUID>> loadResource(const std::vector<std::any> &params) override;

        std::optional<std::shared_ptr<base::Texture>> getResourceByUuid(const std::shared_ptr<base::UUID>& uuid_ptr);

        void unloadResource(const std::vector<std::any> &params) override;

        [[nodiscard]] bool isResourceLoaded(const std::vector<std::any>& params) const override;

        void updateResource(const std::vector<std::any>& params) override;

    private:
        static unsigned int loadTextureFromFile(const char *filePath);
    };
}

#endif // DEMONENGINE_TEXTUREMANAGER_HPP
