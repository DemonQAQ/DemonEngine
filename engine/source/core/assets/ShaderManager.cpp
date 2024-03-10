//
// Created by Demon on 2024/3/9.
//

#include "ShaderManager.hpp"
using namespace assets;

//params1 = vertexPath(string), params2 = fragmentPath(string)
std::optional<base::UUID> ShaderManager::LoadResource(const std::vector<std::any> &params)
{
    if (params.size() < 2)return std::nullopt;

    std::string vertexPath, fragmentPath;
    try
    {
        vertexPath = std::any_cast<std::string>(params[0]);
        fragmentPath = std::any_cast<std::string>(params[1]);
    } catch (const std::bad_any_cast &e)
    {
        std::cerr << "ShaderManager::LoadResource - Bad any_cast: " << e.what() << std::endl;
        return std::nullopt;
    }

    auto shader = std::make_shared<base::Shader>(vertexPath, fragmentPath);
    base::UUID uuid = shader->getUuid();
    shaders[uuid] = shader;
    return uuid;
}

void ShaderManager::UnloadResource(const std::vector<std::any> &params)
{
    if (params.empty() || !params[0].has_value()) return;
    try
    {
        auto uuid = std::any_cast<base::UUID>(params[0]);
        shaders.erase(uuid);
    } catch (const std::bad_any_cast &)
    {
        // 错误处理：提供的参数类型不正确
    }
}

bool ShaderManager::IsResourceLoaded(const std::vector<std::any> &params) const
{
    if (params.size() < 2) return false; // 确保有足够的参数

    try
    {
        auto vertexPath = std::any_cast<std::string>(params[0]);
        auto fragmentPath = std::any_cast<std::string>(params[1]);
        base::UUID uuid(vertexPath + fragmentPath); // 假设UUID构造函数能够从字符串生成UUID

        return shaders.find(uuid) != shaders.end();
    } catch (const std::bad_any_cast &)
    {
        return false;
    }
}

void ShaderManager::UpdateResource(const std::vector<std::any> &params)
{
    if (params.size() < 2) return; // 确保有足够的参数

    try
    {
        auto vertexPath = std::any_cast<std::string>(params[0]);
        auto fragmentPath = std::any_cast<std::string>(params[1]);
        base::UUID uuid(vertexPath + fragmentPath); // 生成UUID

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

std::optional<std::shared_ptr<base::Shader>> ShaderManager::GetResourceByUuid(const base::UUID &uuid)
{

}


