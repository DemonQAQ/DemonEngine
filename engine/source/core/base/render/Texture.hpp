//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_TEXTURE_HPP
#define DEMONENGINE_TEXTURE_HPP

#include <string>
#include <iostream>
#include <utility>
#include <glad/glad.h>
#include <vector>
#include "stb_image.h"
#include "assimp/scene.h"

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

    static TextureType aiTextureTypeToTextureType(aiTextureType aiType)
    {
        switch (aiType)
        {
            case aiTextureType_DIFFUSE:
                return TextureType::Diffuse;
            case aiTextureType_SPECULAR:
                return TextureType::Specular;
            case aiTextureType_HEIGHT:
                return TextureType::Height;
            case aiTextureType_AMBIENT:
                return TextureType::AmbientOcclusion;
            case aiTextureType_EMISSIVE:
                return TextureType::Emissive;
            case aiTextureType_NORMALS:
                return TextureType::Normal;
            default:
                return TextureType::Unknown;
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

    inline const char *toString(TextureType type)
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

    static TextureType fromStringToTextureType(const std::string &typeName)
    {
        if (typeName == "_diffuse") return TextureType::Diffuse;
        else if (typeName == "_specular") return TextureType::Specular;
        else if (typeName == "_normal") return TextureType::Normal;
        else if (typeName == "_height") return TextureType::Height;
        else if (typeName == "_ambientOcclusion") return TextureType::AmbientOcclusion;
        else if (typeName == "_emissive") return TextureType::Emissive;
        else return TextureType::Unknown;
    }

    struct Texture
    {
        unsigned int id;       // 贴图的 OpenGL ID
        std::string type;
        std::string path;

        Texture(unsigned int id, std::string type, std::string path)
                : id(id), type(std::move(type)), path(std::move(path))
        {}

        static unsigned int loadTextureFromFile(const char *path)
        {
            std::string filename = std::string(path);

            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;
            unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                GLint internalFormat = 0;
                GLenum format = 0;
                if (nrComponents == 1)
                {
                    internalFormat = GL_R8; // 对应于GL_RED，使用GL_R8作为内部格式
                    format = GL_RED;
                } else if (nrComponents == 3)
                {
                    internalFormat = GL_RGB8; // 使用GL_RGB8作为内部格式
                    format = GL_RGB;
                } else if (nrComponents == 4)
                {
                    internalFormat = GL_RGBA8; // 使用GL_RGBA8作为内部格式
                    format = GL_RGBA;
                }

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            } else
            {
                std::cerr << "Texture failed to load at path: " << path << std::endl;
                stbi_image_free(data);
            }

            return textureID;
        }

        static Texture loadTextureType(const std::string &path, const std::string &directory, TextureType type)
        {
            std::string filename = std::string(path);
            filename = directory + '/' + filename;
            std::string typeName = toString(type);

            unsigned int textureID = loadTextureFromFile(filename.c_str());
            return Texture(textureID, typeName, filename);
        }
    };
}

#endif //DEMONENGINE_TEXTURE_HPP