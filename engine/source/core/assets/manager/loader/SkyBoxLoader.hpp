//
// Created by Demon on 2024/5/12.
//

#ifndef DEMONENGINE_SKYBOXLOADER_HPP
#define DEMONENGINE_SKYBOXLOADER_HPP

#include <memory>
#include <core/base/render/Shader.hpp>
#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileLoader.hpp"
#include "core/base/scene/skybox/StaticSkybox.hpp"

namespace assets
{
    class SkyBoxLoader : implements IFileLoader
    {
    public:
        static std::shared_ptr<base::StaticSkybox>
        loadStaticSkybox(const std::string &name, const std::string &metaPath, const std::string &texturePath,
                         bool isAssets = false, glm::vec3 emissiveColor = glm::vec3(0.1f, 0.1f, 0.5f), const std::shared_ptr<base::Shader> &shader = nullptr);
    };
}

#endif //DEMONENGINE_SKYBOXLOADER_HPP
