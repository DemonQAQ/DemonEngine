//
// Created by Demon on 2024/3/9.
//

#include <core/io/FileSystem.hpp>
#include <core/assets/AssetType.hpp>
#include <core/base/utils/UUIDUtil.hpp>
#include "ShaderManager.hpp"
#include "ConfigManager.hpp"
#include <filesystem>

using namespace assets;

std::map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Shader>> ShaderManager::loadedShaders;

ShaderManager::ShaderManager()
{
}

/**
 *
 * @params[0] std::shared_ptr<base::UUID> &existingUuid     实例的uuid
 * @params[1] bool init                                     是否第一次创建模型
 * @params[2] std::string                                   shader名称
 * @params[3] std::string                                   顶点着色器的文件路径
 * @params[4] std::string                                   片元着色器的文件路径
 * @params[5] std::shared_ptr<io::YamlConfiguration> &yml   meta文件
 * */
bool ShaderManager::loadData(const std::vector<std::any> &params, bool isAssets)
{
    if (params.size() < 5)
    {
        std::cerr << "Invalid parameters for loadData on ShaderManager." << std::endl;
        return false;
    }

    std::shared_ptr<base::UUID> existingUuid;
    bool init;
    std::string shaderName, vertexPath, fragmentPath;
    std::shared_ptr<io::YamlConfiguration> yml;
    try
    {
        existingUuid = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
        auto it = loadedShaders.find(existingUuid);
        if (it != loadedShaders.end())
        {
            return true;
        }

        init = std::any_cast<bool>(params[1]);
        shaderName = std::any_cast<std::string>(params[2]);
        vertexPath = std::any_cast<std::string>(params[3]);
        fragmentPath = std::any_cast<std::string>(params[4]);
        yml = std::any_cast<std::shared_ptr<io::YamlConfiguration>>(params[5]);

        vertexPath = FileSystem::combinePaths(isAssets ? ASSETS_ROOT_PATH : RESOURCE_PACK_ROOT_PATH, vertexPath);
        fragmentPath = FileSystem::combinePaths(isAssets ? ASSETS_ROOT_PATH : RESOURCE_PACK_ROOT_PATH, fragmentPath);
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "ShaderManager::loadData - Bad any_cast: " << e.what() << std::endl;
        return false;
    }

    auto shader = std::make_shared<base::Shader>(existingUuid, init, shaderName, vertexPath, fragmentPath, yml);
    if (shader)
    {
        loadedShaders[existingUuid] = shader;
        return true;
    }
    else return false;
}

void ShaderManager::unloadData(const std::vector<std::any> &params, bool isAssets)
{
    if (params.empty() || !params[0].has_value()) return;
    try
    {
        auto uuid = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
        loadedShaders.erase(uuid);
    } catch (const std::bad_any_cast &)
    {
        // 错误处理：提供的参数类型不正确
    }
}

bool ShaderManager::isDataLoaded(const std::vector<std::any> &params) const
{
    if (params.size() < 2) return false; // 确保有足够的参数

    try
    {
        auto vertexPath = std::any_cast<std::string>(params[0]);
        auto fragmentPath = std::any_cast<std::string>(params[1]);
        //auto uuid = std::make_shared<base::UUID>(vertexPath + fragmentPath);
        return false;
    } catch (const std::bad_any_cast &)
    {
        return false;
    }
}

void ShaderManager::updateData(const std::vector<std::any> &params)
{
    if (params.size() < 2) return; // 确保有足够的参数

    try
    {
        auto vertexPath = std::any_cast<std::string>(params[0]);
        auto fragmentPath = std::any_cast<std::string>(params[1]);
        //auto uuid = std::make_shared<base::UUID>(vertexPath + fragmentPath);

//        auto it = loadedShaders.find(uuid);
//        if (it != loadedShaders.end())
//        {
//            //todo Shader的reload逻辑
//        }
//        else
//        {
//            // 着色器未找到，可能需要加载新着色器
//            // 这里的具体实现会依据你的需求和Shader类的能力
//        }
    } catch (const std::bad_any_cast &)
    {
        // 错误处理
    }
}

std::optional<std::shared_ptr<base::Shader>>
ShaderManager::getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr)
{
    auto it = loadedShaders.find(uuid_ptr);
    if (it != loadedShaders.end())return it->second;
    else return std::nullopt;
}

void ShaderManager::onStart()
{

}

void ShaderManager::onStop()
{
    std::cerr << "ShaderManager onStop start" << std::endl;
    for (const auto& shader: loadedShaders)shader.second->saveMetadata();
    std::cerr << "ShaderManager onStop end" << std::endl;

}


