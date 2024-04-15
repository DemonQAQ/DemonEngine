//
// Created by Demon on 2024/4/16.
//
#include "UUIDManager.hpp"

using namespace base;

std::unordered_set<std::shared_ptr<base::UUID>, UUIDHash, UUIDEqual, std::allocator<std::shared_ptr<base::UUID>>> UUIDManager::uuids;
std::unordered_set<std::shared_ptr<base::UUID>, UUIDHash, UUIDEqual, std::allocator<std::shared_ptr<base::UUID>>> UUIDManager::reappearUuids;