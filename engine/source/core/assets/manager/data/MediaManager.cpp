﻿//
// Created by Demon on 2024/3/31.
//
#include "MediaManager.hpp"

using namespace assets;

bool MediaManager::loadData(const std::vector<std::any> &params)
{
    return false;
}

void MediaManager::unloadData(const std::vector<std::any> &params)
{

}

bool MediaManager::isDataLoaded(const std::vector<std::any> &params) const
{
    return false;
}

void MediaManager::updateData(const std::vector<std::any> &params)
{

}

void MediaManager::onStart()
{

}

void MediaManager::onStop()
{
    std::cerr << "MediaManager onStop start" << std::endl;
    std::cerr << "MediaManager onStop end" << std::endl;

}
