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
        SHININESS,
        OPACITY,
        DISPLACEMENT,
        REFLECTION,
        LIGHT,
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
            case TextureType::SHININESS:
                return "_shininess";
            case TextureType::OPACITY:
                return "_opacity";
            case TextureType::DISPLACEMENT:
                return "_displacement";
            case TextureType::REFLECTION:
                return "_reflection";
            case TextureType::LIGHT:
                return "_light";
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
        else if (typeName == "_shininess") return base::TextureType::SHININESS;
        else if (typeName == "_opacity") return base::TextureType::OPACITY;
        else if (typeName == "_displacement") return base::TextureType::DISPLACEMENT;
        else if (typeName == "_reflection") return base::TextureType::REFLECTION;
        else if (typeName == "_light") return base::TextureType::LIGHT;
        else return base::TextureType::DIFFUSE;
    }

    class TextureBlockOperator : implements BlockOperator
    {
        void writeToBlock(const std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml)
        {
            yml->set("TextureBlockOperator.textureType", metadata->getValue("textureType"));
        }

        void readFromBlock(const std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml)
        {
            metadata->setValue("textureType", yml->getString("TextureBlockOperator.textureType"));
        }

        void initBlock(std::shared_ptr<Metadata> &metadata, const std::vector<std::any> &params)
        {
            if (!params.empty() && params[0].type() == typeid(TextureType))
            {
                auto textureType_ = std::any_cast<TextureType>(params[0]);
                metadata->setValue("textureType", toString(textureType_));
            }
            else
            {
                metadata->setValue("textureType", toString(TextureType::DIFFUSE));
            }
        }
    };

    class Texture : implements Object, implements IMetaAccessor
    {
    public:
        unsigned int id;       // 贴图的 OpenGL ID
        TextureType type;
        int width, height, channels;
        std::string texturePath;

        void init()
        {
            addOperator(std::make_shared<TextureBlockOperator>());
        }

        Texture(const std::shared_ptr<base::UUID> &existingUuid, bool init,
                std::shared_ptr<io::YamlConfiguration> &yml,
                unsigned int id, TextureType type, const std::string &path, int width, int height, int channels)
                : Object(existingUuid),
                  IMetaAccessor(yml, init, existingUuid),
                  id(id), type(type), texturePath(path), width(width), height(height), channels(channels)
        {}

        [[nodiscard]] std::string getTypeName() const
        {
            return toString(type);
        }
    };
};

#endif //DEMONENGINE_TEXTURE_HPP