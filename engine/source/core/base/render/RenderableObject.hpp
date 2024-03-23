//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_RENDERABLEOBJECT_HPP
#define DEMONENGINE_RENDERABLEOBJECT_HPP

#include "Shader.hpp"
#include <glm/glm.hpp>
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/common/Transform.hpp"
#include "core/base/common/UUID.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"

namespace base
{
    class Model;

    class RenderableObject : implements Object, IRenderable, ITransformableUpdate, INameable
    {
    private:
        std::string name;
        std::vector<Model> models;
    public:
        explicit RenderableObject(
                std::string name,
                const std::vector<Model>& models = {},
                const base::Transform& initialTransform = base::Transform(),
                UUID *shaderUUID = nullptr,
                UUID *materialUUID = nullptr);

        void render()
        {

        }

        void getRenderData(std::vector<RenderData> renderDataList) override;

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

        void beforeRendering(const std::vector<std::any>& params) override;

        void afterRendering(const std::vector<std::any>& params) override;

        void updateGlobalTransform(std::vector<Transform> &additionalTransforms) override;

        void updateObservedGlobalTransform(std::vector<Transform> &additionalTransforms) override;

        void bindShaderToMesh(const std::string &modelName, const std::string &meshName, std::unique_ptr<base::UUID> shader);

        void unbindShaderFromMesh(const std::string &meshName);

        const UUID *getShaderForMesh(const std::string &meshName);
    };
}

#endif //DEMONENGINE_RENDERABLEOBJECT_HPP
