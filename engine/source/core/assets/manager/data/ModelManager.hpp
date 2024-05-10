//
// Created by Demon on 2024/3/6.
//
#ifndef DEMONENGINE_MODELMANAGER_HPP
#define DEMONENGINE_MODELMANAGER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IDataManager.hpp"
#include "TextureManager.hpp"
#include "core/base/render/Model.hpp"
#include "core/base/render/Mesh.hpp"
#include "core/base/render/Vertex.hpp"
#include "core/assets/manager/AssetsDataMainManager.hpp"
#include <vector>
#include <any>
#include <optional>
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include "assimp/scene.h"

namespace assets
{
    class ModelManager : implements IDataManager
    {
    private:
        static std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Model>> loadedModels;
    public:
        /**
         * @params[0] std::shared_ptr<base::UUID> &existingUuid         模型uuid
         * @params[1] bool init                                         是否第一次创建模型
         * @params[2] std::string modelName                             模型名称
         * @params[3] const std::shared_ptr<Node> &root                 模型的组数据
         * @params[4] std::shared_ptr<io::YamlConfiguration> &yml       meta文件
         *
         * 以下为可选项，不传入默认使用nullptr
         * @params[5] const Transform &initialTransform                 模型的变换数据
         * @params[6] const std::shared_ptr<base::UUID> &shaderUUID     使用的shader的uuid
         * @params[7] const std::shared_ptr<base::UUID> &materialUUID   使用的材质的uuid
         * */
        bool loadData(const std::vector<std::any> &params, bool isAssets = false) override;

        void unloadData(const std::vector<std::any> &params, bool isAssets = false) override;

        [[nodiscard]] bool isDataLoaded(const std::vector<std::any> &params) const override;

        void updateData(const std::vector<std::any> &params) override;

        std::optional<std::shared_ptr<base::Model>> getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr);

        void updateModelRelation(const std::shared_ptr<base::UUID> &uuid_ptr);

        void onStart() override;

        void onStop() override;

    private:
        static base::Transform convertAiMatrixToTransform(const aiMatrix4x4 &aiMatrix);
    };
}

#endif //DEMONENGINE_MODELMANAGER_HPP
