//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_RENDERABLEOBJECT_HPP
#define DEMONENGINE_RENDERABLEOBJECT_HPP

#include <glm/glm.hpp>
#include "Shader.hpp"
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/common/Transform.hpp"
#include "core/base/common/UUID.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"
#include "core/base/interface/IRenderable.hpp"

namespace base
{
    class Model;

    class RenderableObject : implements IRenderable,
                             implements Object,
                             implements ITransformableUpdate,
                             implements INameable,
                             implements IMetaAccessor
    {
    private:
        std::string name;
        std::vector<std::shared_ptr<Model>> models;
    public:
        explicit RenderableObject(
                const std::shared_ptr<base::UUID> &existingUuid, bool init, std::string name,
                std::shared_ptr<io::YamlConfiguration> &yml,
                const std::vector<std::shared_ptr<Model>> &models = {},
                const base::Transform &initialTransform = base::Transform(),
                const std::shared_ptr<base::UUID> &shaderUUID = nullptr,
                const std::shared_ptr<base::UUID> &materialUUID = nullptr);

        void render()
        {

        }

        void addModel(const std::shared_ptr<Model> &model);

        void getRenderData(std::vector<RenderData> &renderDataList) override;

        void setName(const std::string &name_) override
        {
            this->name = name_;
        }

        [[nodiscard]] std::string getName() const override
        {
            return name;
        }

        [[nodiscard]] Transform getLocalTransform() const override
        {
            return getTransform();
        }

        void beforeRendering(const std::vector<std::any> &params) override;

        void afterRendering(const std::vector<std::any> &params) override;

        void updateGlobalTransform(std::vector<Transform> &additionalTransforms) override;

        void updateObservedGlobalTransform(std::vector<Transform> &additionalTransforms) override;

        void
        bindShaderToMesh(const std::string &modelName, const std::string &meshName, std::unique_ptr<base::UUID> shader);

        void unbindShaderFromMesh(const std::string &meshName);

        const UUID *getShaderForMesh(const std::string &meshName);
    };
}

#endif //DEMONENGINE_RENDERABLEOBJECT_HPP
