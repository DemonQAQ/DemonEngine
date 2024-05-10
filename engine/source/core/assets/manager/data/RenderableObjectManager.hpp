//
// Created by Demon on 2024/4/20.
//

#ifndef DEMONENGINE_RENDERABLEOBJECTMANAGER_HPP
#define DEMONENGINE_RENDERABLEOBJECTMANAGER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IDataManager.hpp"
#include "core/base/render/RenderableObject.hpp"

namespace assets
{
    class RenderableObjectManager : implements IDataManager
    {
    private:
        static std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::RenderableObject>> loadedRenderableObjects;
    public:
        /**
         * @params[0] std::shared_ptr<base::UUID> &existingUuid         渲染实体uuid
         * @params[1] bool init                                         是否第一次创建
         * @params[2] std::string modelName                             渲染实体名称
         * @params[3] std::vector<std::shared_ptr<Model>> &models       模型的组数据
         * @params[4] std::shared_ptr<io::YamlConfiguration> &yml       meta文件
         * @params[5] base::Transform &initialTransform                 变换数据
         * @params[6] std::shared_ptr<base::UUID> &shaderUUID           使用的shader的uuid
         * @params[7] std::shared_ptr<base::UUID> &materialUUID         使用的材质的uuid
         * */
        bool loadData(const std::vector<std::any> &params, bool isAssets = false) override;

        void unloadData(const std::vector<std::any> &params, bool isAssets = false) override;

        [[nodiscard]] bool isDataLoaded(const std::vector<std::any> &params) const override;

        void updateData(const std::vector<std::any> &params) override;

        std::optional<std::shared_ptr<base::RenderableObject>> getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr);

        void onStart() override;

        void onStop() override;
    };
}

#endif //DEMONENGINE_RENDERABLEOBJECTMANAGER_HPP
