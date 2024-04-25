//
// Created by Demon on 2024/4/20.
//

#ifndef DEMONENGINE_RENDEROBJECTLOADER_HPP
#define DEMONENGINE_RENDEROBJECTLOADER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileLoader.hpp"
#include "core/base/render/RenderableObject.hpp"
#include <iostream>

namespace assets
{
    class RenderObjectLoader : implements IFileLoader
    {
    public:
        static std::shared_ptr<base::RenderableObject>
        loadObject(const std::shared_ptr<base::UUID> &existingUuid, bool init, const std::string& name,
                         std::shared_ptr<io::YamlConfiguration> &yml,
                         const std::vector<std::shared_ptr<base::Model>> &models = {},
                         const base::Transform &initialTransform = base::Transform(),
                         const std::shared_ptr<base::UUID> &shaderUUID = nullptr,
                         const std::shared_ptr<base::UUID> &materialUUID = nullptr);

        static std::shared_ptr<base::RenderableObject> loadObject(std::shared_ptr<io::YamlConfiguration> &yml);
    };
}

#endif //DEMONENGINE_RENDEROBJECTLOADER_HPP
