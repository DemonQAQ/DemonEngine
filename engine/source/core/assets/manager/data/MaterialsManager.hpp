//
// Created by Demon on 2024/3/6.
//

#ifndef DEMONENGINE_MATERIALSMANAGER_HPP
#define DEMONENGINE_MATERIALSMANAGER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IDataManager.hpp"
#include "core/base/render/Material.hpp"
#include "assimp/material.h"
#include "TextureManager.hpp"
#include "core/io/config/JsonConfiguration.hpp"

namespace assets
{
    class MaterialsManager : implements IDataManager
    {
    private:
        static std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Material>> loadedMaterial;
    public:
        MaterialsManager();

        /**
         *
         * @params[0] const std::shared_ptr<base::UUID> &existingUuid   材质的uuid
         * @params[1] bool init                                         是否第一次创建材质
         * @params[2] std::shared_ptr<io::YamlConfiguration> &yml       材质的meta文件
         *
         * 以下为可选参数
         * @params[3] std::string name_                                 材质名称
         * @params[4] glm::vec3 &diffuse_                               漫反射颜色
         * @params[5] glm::vec3 &specular_                              镜面反射颜色
         * @params[6] glm::vec3 &ambient_                               环境光颜色
         * @params[7] glm::vec3 &emissive_                              自发光颜色
         * @params[8] float shininess_                                  光泽度
         * @params[9] float opacity_                                    不透明度
         * @params[10] float roughness_                                  粗糙度
         * @params[11] float metallic_                                  金属度
         * @params[12] float reflectivity_                              反射率
         * @params[13] std::map<TextureType, std::map<std::shared_ptr<base::UUID>, std::shared_ptr<Texture>>> &textures_ 材质使用的贴图
         * */
        bool loadData(const std::vector<std::any> &params) override;

        void unloadData(const std::vector<std::any> &params) override;
        /**
         *
         * @params[0] const std::shared_ptr<base::UUID> &existingUuid   材质的uuid
         * */
        [[nodiscard]] bool isDataLoaded(const std::vector<std::any> &params) const override;

        void updateData(const std::vector<std::any> &params) override;

        std::optional<std::shared_ptr<base::Material>> getResourceByUuid(const std::shared_ptr<base::UUID>& uuid_ptr);

        void updateMaterialRelation(const std::shared_ptr<base::UUID> &uuid_ptr);

        static std::string generateUniqueMaterialName(const aiMaterial *aiMat);

        void onStart() override;

        void onStop() override;
    };
}

#endif //DEMONENGINE_MATERIALSMANAGER_HPP
