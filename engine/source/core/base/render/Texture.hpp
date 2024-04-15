﻿//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_TEXTURE_HPP
#define DEMONENGINE_TEXTURE_HPP

#include "core/base/lib/pch.hpp"
#include "stb_image.h"
#include <utility>
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/BlockOperator.hpp"
#include "core/base/interface/IMetaAccessor.hpp"

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

    class TextureBlockOperator : implements BlockOperator
    {
        void writeToBlock(std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml)
        {

        }

        void readFromBlock(std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml)
        {

        }

        void initBlock(std::shared_ptr<Metadata> &metadata, const std::vector<std::any> &params)
        {

        }
    };

    class Texture : implements Object, implements IMetaAccessor
    {
    public:
        unsigned int id;       // 贴图的 OpenGL ID
        TextureType type;
        std::string texturePath;

        void init()
        {
            addOperator(std::make_shared<TextureBlockOperator>());
        }

        Texture(const std::shared_ptr<base::UUID> &existingUuid, bool init,
                std::shared_ptr<io::YamlConfiguration> &yml,
                unsigned int id, TextureType type, const std::string &path)
                : Object(existingUuid),
                  IMetaAccessor(yml, !init, init ? nullptr : existingUuid),
                  id(id), type(type), texturePath(path)
        {}

        [[nodiscard]] std::string getTypeName() const
        {
            return toString(type);
        }
    };
};

#endif //DEMONENGINE_TEXTURE_HPP