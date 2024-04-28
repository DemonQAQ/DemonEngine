//
// Created by Demon on 2024/4/26.
//

#include <core/assets/AssetType.hpp>
#include <core/assets/manager/data/ScriptManager.hpp>
#include <core/assets/manager/AssetsDataMainManager.hpp>
#include <core/base/utils/UUIDUtil.hpp>
#include <core/base/common/manager/UUIDManager.hpp>
#include "ScriptLoader.hpp"

std::shared_ptr<script::ScriptEntity> assets::ScriptLoader::loadScript(const std::string &path)
{
    auto scriptManagerManagerOpt = AssetsDataMainManager::getManager(AssetType::RENDERABLE_OBJECT);
    if (!scriptManagerManagerOpt.has_value()) return nullptr;

    auto scriptManagerManger = std::dynamic_pointer_cast<ScriptManager>(scriptManagerManagerOpt.value());
    if (!scriptManagerManger) return nullptr;

    auto existingUuid = base::UUIDManager::getUUID(utils::uuidUtil::getUUID());

    if (scriptManagerManger->loadData({existingUuid, path, path}))
    {
        auto renderableObjectOpt = scriptManagerManger->getResourceByUuid(existingUuid);
        if (!renderableObjectOpt.has_value()) return nullptr;

        auto renderableObject = std::dynamic_pointer_cast<script::ScriptEntity>(renderableObjectOpt.value());
        return renderableObject;
    }
    else return nullptr;
}
