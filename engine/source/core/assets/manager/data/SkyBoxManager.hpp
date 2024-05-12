//
// Created by Demon on 2024/5/12.
//

#ifndef DEMONENGINE_SKYBOXMANAGER_HPP
#define DEMONENGINE_SKYBOXMANAGER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IDataManager.hpp"
#include "core/base/interface/ISkyBox.hpp"
#include "core/base/interface/Initializer.hpp"
#include "core/base/render/Shader.hpp"
#include "core/assets/manager/loader/ShaderLoader.hpp"
#include "core/base/scene/skybox/StaticSkybox.hpp"

namespace assets
{
    class SkyBoxManager : implements IDataManager, implements base::Initializer
    {
    private:
        static std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::ISkyBox>> loadedSkyBox;
        static std::unordered_map<base::SkyBoxType, std::shared_ptr<base::Shader>> normalShader;
    public:
        SkyBoxManager();

        ~SkyBoxManager() override;

        /**
         * 创建并管理一个ISkyBox实例
         *
         * @params[0] const std::shared_ptr<base::UUID> &existingUuid   实例的uuid
         * @params[1] bool init                                         是否第一次创建
         * @params[2] std::string &name                                 名称
         * @params[3] std::shared_ptr<io::YamlConfiguration> &yml       实例对应的meta文件
         * @params[4] SkyBoxType skyBoxType                             天空盒类型
         *
         * 根据SkyBoxType决定后续参数的解析逻辑
         * STATIC:
         * @params[5] std::shared_ptr<Texture> &texture                 天空盒贴图
         * 可选:
         * @params[6] glm::vec3 emissiveColor                           环境光颜色
         * @params[7] std::shared_ptr<base::UUID> &shaderUUID           天空盒shader，不传入使用默认天空盒shader
         *
         * PROCEDURAL:
         * 等待实现
         *
         * PANORAMIC:
         * 等待实现
         *
         * DYNAMIC:
         * 等待实现
         * */
        bool loadData(const std::vector<std::any> &params, bool isAssets = false) override;

        void unloadData(const std::vector<std::any> &params, bool isAssets = false) override;

        [[nodiscard]] bool isDataLoaded(const std::vector<std::any> &params) const override;

        void updateData(const std::vector<std::any> &params) override;

        void onStart() override;

        void onStop() override;

        std::optional<std::shared_ptr<base::ISkyBox>> getResourceByUuid(const std::shared_ptr<base::UUID>& uuid_ptr);

        std::shared_ptr<base::Shader> getSkyBoxTypeNormalShader(base::SkyBoxType type);

        bool init(const std::vector<std::any> &params) override;

        void finalize() override;
    private:
        std::shared_ptr<base::StaticSkybox> loadStaticSkyBox(const std::shared_ptr<base::UUID> &existingUuid, bool init,
                                                             const std::string &name, std::shared_ptr<io::YamlConfiguration> &yml,
                                                             const std::vector<std::any> &params);
    };
}

#endif //DEMONENGINE_SKYBOXMANAGER_HPP
