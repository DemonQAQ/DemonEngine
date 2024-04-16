//
// Created by Demon on 2024/3/9.
//
#include <core/io/FileSystem.hpp>
#include <core/base/utils/UUIDUtil.hpp>
#include <core/base/common/manager/UUIDManager.hpp>
#include <core/io/config/YamlConfiguration.hpp>
#include "TextureManager.hpp"
#include "core/assets/manager/AssetsDataMainManager.hpp"
#include "ConfigManager.hpp"

using namespace assets;
using namespace base;

std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Texture>> TextureManager::loadedTextures;

TextureManager::TextureManager()
{
}

/**
 * 创建并管理一个Texture实例
 *
 * @params[0] const std::shared_ptr<base::UUID> &existingUuid   实例的uuid
 * @params[1] bool init                                         是否第一次创建
 * @params[2] std::shared_ptr<io::YamlConfiguration> &yml       实例对应的meta文件
 * @params[3] TextureType type                                  贴图类型
 * @params[4] const std::string &path                           贴图路径
 * */
bool TextureManager::loadData(const std::vector<std::any> &params)
{
    if (params.size() < 5)
    {
        std::cerr << "Invalid parameters for loadData." << std::endl;
        return false;
    }

    std::shared_ptr<base::UUID> existingUuid;
    bool init;
    std::shared_ptr<io::YamlConfiguration> yml;
    TextureType type;
    std::string path;

    try
    {
        existingUuid = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
        auto it = loadedTextures.find(existingUuid);
        if (it != loadedTextures.end())
        {
            return true;
        }

        init = std::any_cast<bool>(params[1]);
        yml = std::any_cast<std::shared_ptr<io::YamlConfiguration>>(params[2]);
        type = std::any_cast<TextureType>(params[3]);
        path = std::any_cast<std::string>(params[4]);
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error extracting parameters: " << e.what() << std::endl;
        return false;
    }

    // 加载纹理
    unsigned int textureID = loadTextureFromFile(path.c_str());
    if (textureID == 0)
    {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        return false;
    }

    loadedTextures[existingUuid] = std::make_shared<base::Texture>(existingUuid, init, yml, textureID, type, path);
    return true;
}

std::optional<std::shared_ptr<base::Texture>>
TextureManager::getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto it = loadedTextures.find(uuid_ptr);
    if (it != loadedTextures.end())return it->second;
    else return std::nullopt;
}

void TextureManager::unloadData(const std::vector<std::any> &params)
{
    if (params.empty())
    {
        std::cerr << "Invalid parameters for unloadData." << std::endl;
        return;
    }

    try
    {
        auto uuid_ptr = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
        auto it = loadedTextures.find(uuid_ptr);
        if (it != loadedTextures.end())
        {
            GLuint textureID = it->second->id;
            glDeleteTextures(1, &textureID);
            loadedTextures.erase(it);
        }
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error extracting UUID parameter: " << e.what() << std::endl;
    }
}

/**
 *
 * @params[0] const std::shared_ptr<base::UUID> &existingUuid   材质的uuid
 * */
[[nodiscard]] bool TextureManager::isDataLoaded(const std::vector<std::any> &params) const
{
    if (params.size() != 1) return false;

    std::shared_ptr<base::UUID> existingUuid;

    try
    {
        existingUuid = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
        auto it = loadedTextures.find(existingUuid);
        if (it != loadedTextures.end())
        {
            return true;
        }
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error extracting parameters: " << e.what() << std::endl;
        return false;
    }
}

void TextureManager::updateData(const std::vector<std::any> &params)
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
