//
// Created by Demon on 2024/4/26.
//

#include <core/assets/AssetType.hpp>
#include <core/assets/manager/data/ScriptManager.hpp>
#include <core/assets/manager/AssetsDataMainManager.hpp>
#include <core/base/utils/UUIDUtil.hpp>
#include <core/base/common/manager/UUIDManager.hpp>
#include "ScriptLoader.hpp"

std::shared_ptr<script::AssemblyScriptEntity> assets::ScriptLoader::loadScript(const std::string &path)
{
    auto scriptManagerManagerOpt = AssetsDataMainManager::getManager(AssetType::SCRIPT);
    if (!scriptManagerManagerOpt.has_value()) return nullptr;

    auto scriptManagerManger = std::dynamic_pointer_cast<ScriptManager>(scriptManagerManagerOpt.value());
    if (!scriptManagerManger) return nullptr;

    auto existingUuid = base::UUIDManager::getUUID(utils::uuidUtil::getUUID());

    if (scriptManagerManger->loadData({existingUuid, path, path}))
    {
        auto scriptEntityOpt = scriptManagerManger->getResourceByUuid(existingUuid);
        if (!scriptEntityOpt.has_value()) return nullptr;

        auto scriptEntity = std::dynamic_pointer_cast<script::AssemblyScriptEntity>(scriptEntityOpt.value());
        return scriptEntity;
    }
    else return nullptr;
}
