//
// Created by Demon on 2024/3/6.
//

#ifndef DEMONENGINE_MATERIALSMANAGER_HPP
#define DEMONENGINE_MATERIALSMANAGER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileManager.hpp"
#include "core/base/render/Material.hpp"
#include "assimp/material.h"
#include "TextureManager.hpp"
#include "core/io/config/JsonConfiguration.hpp"

namespace assets
{
    class MaterialsManager : implements IFileManager
    {
    private:
        static std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Material>> loadedMaterial;
    public:
        MaterialsManager();
        /**
         * 参数传入aiMaterial* aiMat/io::JsonConfiguration& jsonConfig加载单个Material实例并返回UUID
         * */
        std::optional<std::shared_ptr<base::UUID>> loadResource(const std::vector<std::any> &params) override;

        void unloadResource(const std::vector<std::any> &params) override;

        [[nodiscard]] bool isResourceLoaded(const std::vector<std::any> &params) const override;

        void updateResource(const std::vector<std::any> &params) override;

        std::optional<std::shared_ptr<base::Material>> getResourceByUuid(const std::shared_ptr<base::UUID>& uuid_ptr);

        void updateMaterialRelation(const std::shared_ptr<base::UUID> &uuid_ptr);

        static std::string generateUniqueMaterialName(const aiMaterial *aiMat);
    private:
        std::shared_ptr<base::Material> loadMaterialFromAssimp(const aiMaterial* aiMat, const std::string &path);

        std::shared_ptr<base::Material> loadMaterialFromJson(const io::YamlConfiguration& yamlConfig);
    };
}

#endif //DEMONENGINE_MATERIALSMANAGER_HPP
