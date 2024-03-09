//
// Created by Demon on 2024/3/6.
//

#ifndef DEMONENGINE_TEXTUREMANAGER_HPP
#define DEMONENGINE_TEXTUREMANAGER_HPP

#include <unordered_map>
#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileManager.hpp"
#include "core/base/render/Texture.hpp"
#include "assimp/material.h"
#include "glad/glad.h"
#include "stb_image.h"

namespace assets
{
    static base::TextureType aiTextureTypeToTextureType(aiTextureType aiType)
    {
        switch (aiType)
        {
            case aiTextureType_DIFFUSE:
                return base::TextureType::Diffuse;
            case aiTextureType_SPECULAR:
                return base::TextureType::Specular;
            case aiTextureType_HEIGHT:
                return base::TextureType::Height;
            case aiTextureType_AMBIENT:
                return base::TextureType::AmbientOcclusion;
            case aiTextureType_EMISSIVE:
                return base::TextureType::Emissive;
            case aiTextureType_NORMALS:
                return base::TextureType::Normal;
            default:
                return base::TextureType::Unknown;
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

    static base::TextureType fromStringToTextureType(const std::string &typeName)
    {
        if (typeName == "_diffuse") return base::TextureType::Diffuse;
        else if (typeName == "_specular") return base::TextureType::Specular;
        else if (typeName == "_normal") return base::TextureType::Normal;
        else if (typeName == "_height") return base::TextureType::Height;
        else if (typeName == "_ambientOcclusion") return base::TextureType::AmbientOcclusion;
        else if (typeName == "_emissive") return base::TextureType::Emissive;
        else return base::TextureType::Unknown;
    }

    class TextureManager : implements IFileManager
    {
    private:
        static std::unordered_map<base::UUID, std::shared_ptr<base::Texture>> loadedTextures;

    public:
        //params1 = path(string), params2 = textureType(base::TextureType)
        std::optional<base::UUID> LoadResource(const std::vector<std::any> &params) override;

        std::optional<std::shared_ptr<base::Texture>> GetResourceByUuid(const base::UUID &uuid);

        void UnloadResource(const std::vector<std::any> &params) override;

        [[nodiscard]] bool IsResourceLoaded(const std::string &path) const override;

        void UpdateResource(const std::string &path) override;

    private:
        static unsigned int loadTextureFromFile(const char *filePath);
    };

    std::unordered_map<base::UUID, std::shared_ptr<base::Texture>> TextureManager::loadedTextures;
}

#endif // DEMONENGINE_TEXTUREMANAGER_HPP
