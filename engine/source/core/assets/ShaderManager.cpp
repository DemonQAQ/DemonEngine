//
// Created by Demon on 2024/3/9.
//
#include "ShaderManager.hpp"

std::optional<base::UUID> assets::ShaderManager::LoadResource(const std::vector<std::any> &params)
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

void assets::ShaderManager::UnloadResource(const std::vector<std::any> &params)
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

bool assets::ShaderManager::IsResourceLoaded(const std::vector<std::any>& params) const
{

}

void assets::ShaderManager::UpdateResource(const std::vector<std::any>& params)
{

}

