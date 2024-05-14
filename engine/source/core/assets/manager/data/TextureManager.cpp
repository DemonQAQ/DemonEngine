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
    stbi_set_flip_vertically_on_load(true);
}

/**
 * 创建并管理一个Texture实例
 * 当isCube=true时会加载会尝试传入的文件名+_front,_back,_top,_bottom,_right,_left六面的贴图
 * 例如传入xxx/name.png,会尝试加载name_front.png
 *
 * @params[0] const std::shared_ptr<base::UUID> &existingUuid   实例的uuid
 * @params[1] bool init                                         是否第一次创建
 * @params[2] std::shared_ptr<io::YamlConfiguration> &yml       实例对应的meta文件
 * @params[3] TextureType type                                  贴图类型
 * @params[4] const std::string &path                           贴图路径
 * @params[5] bool isCube                                       是否为立方体贴图
 * */
bool TextureManager::loadData(const std::vector<std::any> &params, bool isAssets)
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
    bool isCube = false;

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
        if (params.size() >= 6)isCube = std::any_cast<bool>(params[5]);
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error extracting parameters: " << e.what() << std::endl;
        return false;
    }

    std::tuple<unsigned int, int, int, int> returnValue;
    if (isCube)
    {
        std::string fullPath = FileSystem::extractDirectory(path);
        std::string completeFilename = FileSystem::getFilenameWithoutExtension(path);
        std::string fileExtension = FileSystem::getFullExtension(path);
        returnValue = loadCubeTextureFromFile(fullPath.c_str(), completeFilename.c_str(), fileExtension.c_str());
    }
    else returnValue = loadTextureFromFile(path.c_str());

    if (std::get<0>(returnValue) == 0)
    {
        std::cerr << "Texture failed to load at path: " << path << std::endl;
        return false;
    }

    loadedTextures[existingUuid] = std::make_shared<base::Texture>(existingUuid, init, yml, std::get<0>(returnValue), type, path,
                                                                   std::get<1>(returnValue), std::get<2>(returnValue),
                                                                   std::get<3>(returnValue));
    return true;
}

std::optional<std::shared_ptr<base::Texture>>
TextureManager::getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto it = loadedTextures.find(uuid_ptr);
    if (it != loadedTextures.end())return it->second;
    else return std::nullopt;
}

void TextureManager::unloadData(const std::vector<std::any> &params, bool isAssets)
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
    return false;
}

void TextureManager::updateData(const std::vector<std::any> &params)
{

}

std::tuple<unsigned int, int, int, int> TextureManager::loadTextureFromFile(const char *filePath)
{
    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(false);
    unsigned char *data = stbi_load(filePath, &width, &height, &nrComponents, 0);
    if (!data)
    {
        std::cerr << "Texture failed to load at path: " << filePath << std::endl;
        return std::make_tuple(0, 0, 0, 0);
    }

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format;
    switch (nrComponents)
    {
        case 1:
            format = GL_RED;
            break;
        case 2:
            format = GL_RG;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            std::cerr << "Unsupported texture format with " << nrComponents << " components." << std::endl;
            stbi_image_free(data);
            glDeleteTextures(1, &textureID);
            return std::make_tuple(0, 0, 0, 0);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
    return std::make_tuple(textureID, width, height, nrComponents);
}

std::tuple<unsigned int, int, int, int>
TextureManager::loadCubeTextureFromFile(const char *folderPath, const char *filePath, const char *fileTypeName)
{
    int width = 0, height = 0, nrChannels = 0;
    stbi_set_flip_vertically_on_load(false);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    GLenum faces[6] = {
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    std::string type[6] = {
            "_right",
            "_left",
            "_top",
            "_bottom",
            "_back",
            "_front"
    };

    for (unsigned int i = 0; i < 6; i++)
    {
        std::string fileName = std::string(folderPath) + "/" + std::string(filePath) + type[i] + std::string(fileTypeName);
        unsigned char *data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
        if (!data)
        {
            std::cerr << "Cubemap texture failed to load at path: " << fileName << std::endl;
            glDeleteTextures(1, &textureID);
            return std::make_tuple(0, 0, 0, 0);
        }
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(faces[i], 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return std::make_tuple(textureID, width, height, nrChannels);
}

void TextureManager::onStart()
{

}

void TextureManager::onStop()
{
    std::cerr << "TextureManager onStop start" << std::endl;
    for (const auto &texture: loadedTextures)texture.second->saveMetadata();
    std::cerr << "TextureManager onStop end" << std::endl;

}


