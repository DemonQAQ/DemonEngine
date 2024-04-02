//
// Created by Demon on 2024/3/31.
//
#include "MediaManager.hpp"

using namespace assets;

std::optional<std::shared_ptr<base::UUID>> MediaManager::loadResource(const std::vector<std::any> &params)
{
    return std::nullopt;
}

void MediaManager::unloadResource(const std::vector<std::any> &params)
{

}

bool MediaManager::isResourceLoaded(const std::vector<std::any> &params) const
{
    return false;
}

void MediaManager::updateResource(const std::vector<std::any> &params)
{

}
