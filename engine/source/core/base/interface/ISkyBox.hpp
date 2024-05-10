//
// Created by Demon on 2024/5/11.
//

#ifndef DEMONENGINE_ISKYBOX_HPP
#define DEMONENGINE_ISKYBOX_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/interface/IRenderable.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"

namespace base
{
    enum SkyBoxType
    {
        STATIC,
        PROCEDURAL,
        PANORAMIC,
        DYNAMIC
    };

    interface ISkyBox : implements IRenderable,
                        implements Object,
                        implements IRenderUpdatable,
                        implements INameable,
                        implements base::IMetaAccessor
    {
    protected:
        std::string name;
        SkyBoxType skyBoxType;
    public:
        ISkyBox(const std::shared_ptr<base::UUID> &existingUuid, bool init,
                std::string modelName, std::shared_ptr<io::YamlConfiguration> &yml,
                SkyBoxType skyBoxType,
                const std::shared_ptr<base::UUID> &shaderUUID = nullptr,
                const std::shared_ptr<base::UUID> &materialUUID = nullptr)
                : Object(existingUuid),
                  IMetaAccessor(yml, init, existingUuid),
                  name(std::move(modelName)), skyBoxType(skyBoxType)
        {
            if (shaderUUID)bindShader(shaderUUID);
            else bindShader(getDefaultShader());

            if (materialUUID)bindMaterial(materialUUID);
            else bindMaterial(getDefaultMaterial());
        }

        void setName(const std::string &name_) override
        {
            name = name_;
        }

        [[nodiscard]] std::string getName() const override
        {
            return name;
        }

        virtual void beforeRendering(const std::vector<std::any> &params) override = 0;

        virtual void afterRendering(const std::vector<std::any> &params) override = 0;

        virtual void getRenderData(std::vector<RenderData> &renderDataList) override = 0;
    };
}

#endif //DEMONENGINE_ISKYBOX_HPP
