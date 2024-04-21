//
// Created by Demon on 2024/4/20.
//

#include "RenderObjectLoader.hpp"
#include "core/assets/manager/AssetsDataMainManager.hpp"
#include "core/assets/manager/data/RenderableObjectManager.hpp"

std::shared_ptr<base::RenderableObject>
assets::RenderObjectLoader::loadObject(const std::shared_ptr<base::UUID> &existingUuid, bool init,
                                             const std::string &name, std::shared_ptr<io::YamlConfiguration> &yml,
                                             const std::vector<std::shared_ptr<base::Model>> &models,
                                             const base::Transform &initialTransform,
                                             const std::shared_ptr<base::UUID> &shaderUUID,
                                             const std::shared_ptr<base::UUID> &materialUUID)
{
    auto renderObjectManagerOpt = AssetsDataMainManager::getManager(AssetType::RENDERABLE_OBJECT);
    if (!renderObjectManagerOpt.has_value()) return nullptr;

    auto renderableObjectManger = std::dynamic_pointer_cast<RenderableObjectManager>(renderObjectManagerOpt.value());
    if (!renderableObjectManger) return nullptr;

    if (renderableObjectManger->loadData(
            {existingUuid, init, name, yml, models, initialTransform, shaderUUID, materialUUID}))
    {
        auto renderableObjectOpt = renderableObjectManger->getResourceByUuid(existingUuid);
        if (!renderableObjectOpt.has_value()) return nullptr;

        auto renderableObject = std::dynamic_pointer_cast<base::RenderableObject>(renderableObjectOpt.value());
        return renderableObject;
    }
    else return nullptr;
}

std::shared_ptr<base::RenderableObject>
assets::RenderObjectLoader::loadObject(std::shared_ptr<io::YamlConfiguration> &yml)
{
    return nullptr;
}
