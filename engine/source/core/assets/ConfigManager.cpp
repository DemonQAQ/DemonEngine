//
// Created by Demon on 2024/3/10.
//

#include "ConfigManager.hpp"

using namespace assets;

std::optional<base::UUID> assets::ConfigManager::LoadResource(const std::vector<std::any> &params)
{
    return std::nullopt;
}

void ConfigManager::UnloadResource(const std::vector<std::any> &params)
{

}

bool ConfigManager::IsResourceLoaded(const std::vector<std::any> &params) const
{
    return false;
}

void ConfigManager::UpdateResource(const std::vector<std::any> &params)
{

}
