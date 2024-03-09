//
// Created by Demon on 2024/3/9.
//
#include "TextureManager.hpp"

using namespace assets;

//params1 = path(string), params2 = textureType(base::TextureType)
std::optional<base::UUID> TextureManager::LoadResource(const std::vector<std::any> &params)
{
    if (params.size() < 2)
    {
        std::cerr << "Invalid parameters for LoadResource." <<
                  std::endl;
        return
                std::nullopt;
    }

    std::string path;
    base::TextureType type;

    try
    {
        path = std::any_cast<std::string>(params[0]);
        type = std::any_cast<base::TextureType>(params[1]);
    } catch (
            const std::bad_any_cast &e
    )
    {
        std::cerr << "Error extracting parameters: " << e.what() << std::endl;
        return std::nullopt;
    }

    base::UUID textureUuid(path);
    auto it = loadedTextures.find(textureUuid.toString());
    if (it != loadedTextures.end())
    {
        return it->first;
    }

// 加载纹理
    unsigned int textureID = loadTextureFromFile(path.c_str());
    if (textureID == 0)
    {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        return std::nullopt;
    }

    loadedTextures[textureUuid] = std::make_shared<base::Texture>(textureID, type, path, textureUuid);
    return textureUuid;
}

std::optional<std::shared_ptr<base::Texture>> TextureManager::GetResourceByUuid(const base::UUID &uuid)
{
    auto it = loadedTextures.find(uuid);
    if (it != loadedTextures.end())return it->second;
    else return std::nullopt;
}

void TextureManager::UnloadResource(const std::vector<std::any> &params)
{
    if (params.empty())
    {
        std::cerr << "Invalid parameters for UnloadResource." << std::endl;
        return;
    }

    base::UUID uuid;
    try
    {
        uuid = std::any_cast<base::UUID>(params[0]);
    }
    catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error extracting UUID parameter: " << e.what() << std::endl;
        return;
    }

    auto it = loadedTextures.find(uuid);
    if (it != loadedTextures.end())
    {
        GLuint textureID = it->second->id;
        glDeleteTextures(1, &textureID);
        loadedTextures.erase(it);
    }
}

[[nodiscard]] bool TextureManager::IsResourceLoaded(const std::string &path) const
{

}

void TextureManager::UpdateResource(const std::string &path)
{

}

unsigned int TextureManager::loadTextureFromFile(const char *filePath)
{
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filePath, &width, &height, &nrComponents, 0);
    if (!data)
    {
        std::cerr << "Texture failed to load at path: " << filePath << std::endl;
        return 0;
    }

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return textureID;
}