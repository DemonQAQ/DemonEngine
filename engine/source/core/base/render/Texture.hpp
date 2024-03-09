//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_TEXTURE_HPP
#define DEMONENGINE_TEXTURE_HPP

#include <utility>

#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"

namespace base
{
    enum class TextureType
    {
        Diffuse,
        Specular,
        Normal,
        Height,
        AmbientOcclusion,
        Emissive,
        Unknown
    };

    inline const char *toString(base::TextureType type)
    {
        switch (type)
        {
            case TextureType::Diffuse:
                return "_diffuse";
            case TextureType::Specular:
                return "_specular";
            case TextureType::Normal:
                return "_normal";
            case TextureType::Height:
                return "_height";
            case TextureType::AmbientOcclusion:
                return "_ambientOcclusion";
            case TextureType::Emissive:
                return "_emissive";
            default:
                return "";
        }
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

        Texture(unsigned int id, TextureType type, std::string path, const UUID &uuid)
                : Object(uuid), id(id), type(type), texturePath(std::move(path))
        {}
    };
};

#endif //DEMONENGINE_TEXTURE_HPP