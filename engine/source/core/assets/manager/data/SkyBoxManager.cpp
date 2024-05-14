//
// Created by Demon on 2024/5/12.
//

#include "SkyBoxManager.hpp"
#include "core/base/scene/skybox/StaticSkybox.hpp"

std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::ISkyBox>> assets::SkyBoxManager::loadedSkyBox;
std::unordered_map<base::SkyBoxType, std::shared_ptr<base::Shader>> assets::SkyBoxManager::normalShader;

assets::SkyBoxManager::SkyBoxManager()
{

}

assets::SkyBoxManager::~SkyBoxManager()
{

}

/**
 * 创建并管理一个ISkyBox实例
 *
 * @params[0] const std::shared_ptr<base::UUID> &existingUuid   实例的uuid
 * @params[1] bool init                                         是否第一次创建
 * @params[2] std::string &name                                 名称
 * @params[3] std::shared_ptr<io::YamlConfiguration> &yml       实例对应的meta文件
 * @params[4] SkyBoxType skyBoxType                             天空盒类型
 *
 * 根据SkyBoxType决定后续参数的解析逻辑
 * STATIC:
 * @params[5] std::shared_ptr<Texture> &texture                 天空盒贴图
 * 可选:
 * @params[6] glm::vec3 emissiveColor                           环境光颜色
 * @params[7] std::shared_ptr<base::UUID> &shaderUUID           天空盒shader，不传入使用默认天空盒shader
 *
 * PROCEDURAL:
 * 等待实现
 *
 * PANORAMIC:
 * 等待实现
 *
 * DYNAMIC:
 * 等待实现
 * */
bool assets::SkyBoxManager::loadData(const std::vector<std::any> &params, bool isAssets)
{
    if (params.size() <= 5)
    {
        std::cerr << "Invalid parameters for loadData on SkyBoxManager." << std::endl;
        return false;
    }

    std::shared_ptr<base::UUID> existingUuid;
    bool init;
    std::string name;
    std::shared_ptr<io::YamlConfiguration> yml;
    base::SkyBoxType skyBoxType;

    try
    {
        existingUuid = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
        auto it = loadedSkyBox.find(existingUuid);
        if (it != loadedSkyBox.end())
        {
            return true;
        }

        init = std::any_cast<bool>(params[1]);
        name = std::any_cast<std::string>(params[2]);
        yml = std::any_cast<std::shared_ptr<io::YamlConfiguration>>(params[3]);
        skyBoxType = std::any_cast<base::SkyBoxType>(params[4]);
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error extracting parameters: " << e.what() << std::endl;
        return false;
    }

    std::shared_ptr<base::ISkyBox> iSkyBox = nullptr;

    switch (skyBoxType)
    {
        case base::STATIC:
            auto staticSkyBox = loadStaticSkyBox(existingUuid, init, name, yml, params);
            if (staticSkyBox)iSkyBox = std::dynamic_pointer_cast<base::ISkyBox>(staticSkyBox);
            break;
    }

    if (iSkyBox)
    {
        loadedSkyBox[iSkyBox->getUUID()] = iSkyBox;
        return true;
    }
    else return false;
}

std::shared_ptr<base::StaticSkybox> assets::SkyBoxManager::loadStaticSkyBox(const std::shared_ptr<base::UUID> &existingUuid, bool init,
                                                                            const std::string &name,
                                                                            std::shared_ptr<io::YamlConfiguration> &yml,
                                                                            const std::vector<std::any> &params)
{
    if (params.size() < 6)
    {
        std::cerr << "Invalid parameters for loadStaticSkyBox." << std::endl;
        return nullptr;
    }

    std::shared_ptr<base::Texture> texture = nullptr;
    glm::vec3 emissiveColor;
    std::shared_ptr<base::UUID> shaderUUID;
    try
    {
        texture = std::any_cast<std::shared_ptr<base::Texture>>(params[5]);

        if (params.size() >= 7) emissiveColor = std::any_cast<glm::vec3>(params[6]);
        else emissiveColor = glm::vec3(0.1f, 0.1f, 0.5f);

        if (params.size() >= 8) shaderUUID = std::any_cast<std::shared_ptr<base::UUID>>(params[7]);
        else shaderUUID = nullptr;
    }
    catch (const std::bad_any_cast &e)
    {
        std::cerr << "Error extracting parameters: " << e.what() << std::endl;
        return nullptr;
    }
    return std::make_shared<base::StaticSkybox>(existingUuid, init, name, yml, base::SkyBoxType::STATIC, texture, emissiveColor,
                                                shaderUUID);
}

void assets::SkyBoxManager::unloadData(const std::vector<std::any> &params, bool isAssets)
{

}

bool assets::SkyBoxManager::isDataLoaded(const std::vector<std::any> &params) const
{
    return false;
}

void assets::SkyBoxManager::updateData(const std::vector<std::any> &params)
{

}

void assets::SkyBoxManager::onStart()
{
    auto staticSkyBoxNormalShader = assets::ShaderLoader::loadShader("staticSkyBoxNormalShader", "shader/skybox",
                                                                     "shader/skybox/staticSkyBoxNormalShader.vsh",
                                                                     "shader/skybox/staticSkyBoxNormalShader.fsh", true);
    normalShader[base::SkyBoxType::STATIC] = staticSkyBoxNormalShader;
}

void assets::SkyBoxManager::onStop()
{
    normalShader.clear();
    loadedSkyBox.clear();
}

std::shared_ptr<base::Shader> assets::SkyBoxManager::getSkyBoxTypeNormalShader(base::SkyBoxType type)
{
    return normalShader[base::SkyBoxType::STATIC];
}

std::optional<std::shared_ptr<base::ISkyBox>> assets::SkyBoxManager::getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto it = loadedSkyBox.find(uuid_ptr);
    if (it != loadedSkyBox.end())return it->second;
    else return std::nullopt;
}
