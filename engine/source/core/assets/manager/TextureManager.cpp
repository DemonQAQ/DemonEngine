//
// Created by Demon on 2024/3/9.
//
#include <core/io/FileSystem.hpp>
#include <core/base/utils/UUIDUtil.hpp>
#include <core/base/common/manager/UUIDManager.hpp>
#include <core/io/config/YamlConfiguration.hpp>
#include "TextureManager.hpp"
#include "AssetsMainManager.hpp"
#include "ConfigManager.hpp"

using namespace assets;

std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Texture>> TextureManager::loadedTextures;

TextureManager::TextureManager()
{
}

/**
 * @params[0] std::string 资源文件架路径下的资源地址
 * @params[1] TextureType 贴图类型
 * */
std::optional<std::shared_ptr<base::UUID>> TextureManager::loadResource(const std::vector<std::any> &params)
{
    if (params.size() < 2)
    {
        std::cerr << "Invalid parameters for loadResource." <<std::endl;
        return std::nullopt;
    }

    std::string sourcePath;
    std::string metadataPath;
    base::TextureType type;

    try
    {
        sourcePath = std::any_cast<std::string>(params[0]);
        type = std::any_cast<base::TextureType>(params[1]);
        sourcePath = FileSystem::combinePaths(SOURCE_ROOT_PATH, sourcePath);
        metadataPath = sourcePath + ".meta";
    } catch (
            const std::bad_any_cast &e
    )
    {
        std::cerr << "Error extracting parameters: " << e.what() << std::endl;
        return std::nullopt;
    }

    auto configManagerOpt = AssetsMainManager::getManager(AssetType::CONFIG);
    if (!configManagerOpt.has_value()) return nullptr;
    auto configManager = std::dynamic_pointer_cast<ConfigManager>(configManagerOpt.value());
    if (!configManager) return nullptr;

    auto metaUuid = configManager->loadResource({metadataPath});
    if (!metaUuid.has_value())
    {
        std::cerr << "Failed to load metadata from: " << metadataPath << std::endl;
        return std::nullopt;
    }

    auto metaFileOpt = configManager->getResourceByUuid(metaUuid.value());
    if (!metaFileOpt.has_value())
    {
        std::cerr << "Failed to find metadata file with UUID: " << metaUuid.value()->toString() << std::endl;
        return std::nullopt;
    }

    auto metaFile = std::dynamic_pointer_cast<io::YamlConfiguration>(metaFileOpt.value());
    if (!metaFile)
    {
        std::cerr << "Metadata file is not a YAML configuration." << std::endl;
        return std::nullopt;
    }

    std::string uuidStr = metaFile->getString("uuid");
    bool init = uuidStr.empty();
    if (init) uuidStr = utils::uuidUtil::getUUID(sourcePath);
    auto textureUuid = UUIDManager::getUUID(uuidStr);
    auto it = loadedTextures.find(textureUuid);
    if (it != loadedTextures.end())
    {
        return it->first;
    }

    // 加载纹理
    unsigned int textureID = loadTextureFromFile(sourcePath.c_str());
    if (textureID == 0)
    {
        std::cerr << "Texture failed to load at path: " << sourcePath << std::endl;
        return std::nullopt;
    }

    loadedTextures[textureUuid] = std::make_shared<base::Texture>(textureUuid, init, metaFile, textureID, type,
                                                                  sourcePath);
    return textureUuid;
}

std::optional<std::shared_ptr<base::Texture>>
TextureManager::getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto it = loadedTextures.find(uuid_ptr);
    if (it != loadedTextures.end())return it->second;
    else return std::nullopt;
}

void TextureManager::unloadResource(const std::vector<std::any> &params)
{
    if (params.empty())
    {
        std::cerr << "Invalid parameters for unloadResource." << std::endl;
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

[[nodiscard]] bool TextureManager::isResourceLoaded(const std::vector<std::any> &params) const
{
    return false;
}

void TextureManager::updateResource(const std::vector<std::any> &params)
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
