//
// Created by Demon on 2024/3/9.
//

#include "ShaderManager.hpp"
using namespace assets;

std::map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Shader>> ShaderManager::shaders;

//params1 = vertexPath(string), params2 = fragmentPath(string)
std::optional<std::shared_ptr<base::UUID>> ShaderManager::loadResource(const std::vector<std::any> &params)
{
    if (params.size() < 2)return std::nullopt;

    std::string vertexPath, fragmentPath;
    try
    {
        vertexPath = std::any_cast<std::string>(params[0]);
        fragmentPath = std::any_cast<std::string>(params[1]);
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "ShaderManager::loadResource - Bad any_cast: " << e.what() << std::endl;
        return std::nullopt;
    }

    auto shader = std::make_shared<base::Shader>(vertexPath, fragmentPath);
    auto uuid = shader->getUUID();
    shaders[uuid] = shader;
    return uuid;
}

void ShaderManager::unloadResource(const std::vector<std::any> &params)
{
    if (params.empty() || !params[0].has_value()) return;
    try
    {
        auto uuid = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
        shaders.erase(uuid);
    } catch (const std::bad_any_cast &)
    {
        // 错误处理：提供的参数类型不正确
    }
}

bool ShaderManager::isResourceLoaded(const std::vector<std::any> &params) const
{
    if (params.size() < 2) return false; // 确保有足够的参数

    try
    {
        auto vertexPath = std::any_cast<std::string>(params[0]);
        auto fragmentPath = std::any_cast<std::string>(params[1]);
        auto uuid = std::make_shared<base::UUID>(vertexPath + fragmentPath);
        return shaders.find(uuid) != shaders.end();
    } catch (const std::bad_any_cast &)
    {
        return false;
    }
}

void ShaderManager::updateResource(const std::vector<std::any> &params)
{
    if (params.size() < 2) return; // 确保有足够的参数

    try
    {
        auto vertexPath = std::any_cast<std::string>(params[0]);
        auto fragmentPath = std::any_cast<std::string>(params[1]);
        auto uuid = std::make_shared<base::UUID>(vertexPath + fragmentPath);

        auto it = shaders.find(uuid);
        if (it != shaders.end())
        {
            //todo Shader的reload逻辑
        } else
        {
            // 着色器未找到，可能需要加载新着色器
            // 这里的具体实现会依据你的需求和Shader类的能力
        }
    } catch (const std::bad_any_cast &)
    {
        // 错误处理
    }
}

std::optional<std::shared_ptr<base::Shader>> ShaderManager::getResourceByUuid(const std::shared_ptr<base::UUID>& uuid_ptr)
{
    auto it = shaders.find(uuid_ptr);
    if (it != shaders.end())return it->second;
    else return std::nullopt;
}


