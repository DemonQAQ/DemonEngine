//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_TEXTURE_HPP
#define DEMONENGINE_TEXTURE_HPP

#include "core/base/lib/pch.hpp"
#include "stb_image.h"
#include <utility>
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"

namespace base
{
    enum class TextureType
    {
        DIFFUSE,
        SPECULAR,
        NORMAL,
        HEIGHT,
        AMBIENT_OCCLUSION,
        EMISSIVE,
        UNKNOWN
    };

    inline const char *toString(base::TextureType type)
    {
        switch (type)
        {
            case TextureType::DIFFUSE:
                return "_diffuse";
            case TextureType::SPECULAR:
                return "_specular";
            case TextureType::NORMAL:
                return "_normal";
            case TextureType::HEIGHT:
                return "_height";
            case TextureType::AMBIENT_OCCLUSION:
                return "_ambientOcclusion";
            case TextureType::EMISSIVE:
                return "_emissive";
            default:
                return "";
        }
    }

    static base::TextureType fromStringToTextureType(const std::string &typeName)
    {
        if (typeName == "_diffuse") return base::TextureType::DIFFUSE;
        else if (typeName == "_specular") return base::TextureType::SPECULAR;
        else if (typeName == "_normal") return base::TextureType::NORMAL;
        else if (typeName == "_height") return base::TextureType::HEIGHT;
        else if (typeName == "_ambientOcclusion") return base::TextureType::AMBIENT_OCCLUSION;
        else if (typeName == "_emissive") return base::TextureType::EMISSIVE;
        else return base::TextureType::UNKNOWN;
    }

    class Texture : implements Object
    {
    public:
        unsigned int id;       // 贴图的 OpenGL ID
        TextureType type;
        std::string texturePath;

        Texture(unsigned int id, TextureType type, const std::string &path)
                : Object(path), id(id), type(type), texturePath(path)
        {}

        Texture(unsigned int id, TextureType type, std::string path, const std::shared_ptr<base::UUID> &uuid)
                : Object(uuid), id(id), type(type), texturePath(std::move(path))
        {}

        [[nodiscard]] std::string getTypeName() const
        {
            return toString(type);
        }
    };
};

#endif //DEMONENGINE_TEXTURE_HPP