//
// Created by Demon on 2024/5/10.
//

#ifndef DEMONENGINE_ICOMMONSHAPE_HPP
#define DEMONENGINE_ICOMMONSHAPE_HPP

#include <memory>
#include "core/base/interface/Interface.hpp"
#include "core/base/interface/IRenderable.hpp"
#include "core/base/interface/Initializer.hpp"
#include "core/base/render/Model.hpp"
#include "core/base/render/Mesh.hpp"

namespace base
{
    enum ShapeType
    {
        ANGULAR_SPHERE,
        ANNULUS,
        CUBE,
        CYLINDER,
        CENTRUM,
        MONKEY,
        PLANE,
        SPHERE
    };

    interface ICommonShape :
            implements base::IRenderable,
            implements base::Object,
            implements base::ITransformableUpdate,
            implements base::INameable,
            implements base::IMetaAccessor
    {
    protected:
        std::string name;
        std::vector<std::shared_ptr<base::Mesh>> meshes;
    public:
        ICommonShape(const std::shared_ptr<base::UUID> &existingUuid, bool init,
                     std::string modelName, std::shared_ptr<io::YamlConfiguration> &yml,
                     const base::Transform &initialTransform = base::Transform(),
                     const std::shared_ptr<base::UUID> &shaderUUID = nullptr,
                     const std::shared_ptr<base::UUID> &materialUUID = nullptr)
                : Object(existingUuid),
                  IMetaAccessor(yml, init, existingUuid),
                  name(std::move(modelName))
        {
            if (shaderUUID)bindShader(shaderUUID);
            else bindShader(getDefaultShader());

            if (materialUUID)bindMaterial(materialUUID);
            else bindMaterial(getDefaultMaterial());
            setTransform(initialTransform);
            meshes = {};
        }

        void setName(const std::string &name_) override
        {
            name = name_;
        }

        [[nodiscard]] std::string getName() const override
        {
            return name;
        }

        void beforeRendering(const std::vector<std::any> &params) override
        {
            if (updated)return;
            std::vector<base::Transform> additionalTransforms;
            if (!params.empty())
            {
                if (params[0].type() == typeid(const std::vector<base::Transform>))
                    additionalTransforms = std::any_cast<std::vector<base::Transform>>(params[0]);
                updateGlobalTransform(additionalTransforms);
            }
            updated = true;
        }

        void afterRendering(const std::vector<std::any> &params) override
        {
            updated = false;
        }

        void updateGlobalTransform(std::vector<base::Transform> &additionalTransforms) override
        {
            if (!isTransformDirty() || additionalTransforms.empty())return;
            setTransformDirty(false);
            updateSelfGlobalTransform(additionalTransforms);
            updateObservedGlobalTransform(additionalTransforms);
        }

        void getRenderData(std::vector<base::RenderData> &renderDataList) override
        {
            for (auto &mesh: meshes)
            {
                mesh->getRenderData(renderDataList);
            }
        }

        [[nodiscard]] base::Transform getLocalTransform() const override
        {
            return getTransform();
        }

    private:
        void updateObservedGlobalTransform(std::vector<base::Transform> &additionalTransforms) override
        {
            std::vector<base::Transform> transformsToMerge = {getLocalTransform()};
            transformsToMerge.insert(transformsToMerge.end(), additionalTransforms.begin(), additionalTransforms.end());
            for (const auto &mesh: meshes)mesh->updateGlobalTransform(transformsToMerge);
        }
    };
}

#endif //DEMONENGINE_ICOMMONSHAPE_HPP
