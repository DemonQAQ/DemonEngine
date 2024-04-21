//
// Created by Demon on 2024/3/27.
//

#ifndef DEMONENGINE_ENTITY_HPP
#define DEMONENGINE_ENTITY_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"
#include "core/base/render/RenderableObject.hpp"
#include "core/io/interface/ISerializable.hpp"

using namespace base;

namespace assets::scene
{
    //todo 事件系统接口，允许实体收发事件
    //todo 网络同步系统接口
    //todo 可序列化接口
    //todo 物理系统接口
    class Entity
            : implements Object,
              implements INameable,
              implements ITransformableUpdate,
              implements io::ISerializable,
              implements IMetaAccessor
    {
    private:
        std::string name;
        std::shared_ptr<base::RenderableObject> renderableObject;
        //todo 组件数据，包括碰撞箱、脚本、触发器、音效、特效
    public:
        explicit Entity(const std::shared_ptr<base::UUID> &existingUuid,
                        bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                        std::string name = "Entity");

        void addModel(const std::shared_ptr<Model> &model);

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        [[nodiscard]] std::string Serialize() const override;

        void Deserialize(const std::string &data) override;

        void beforeRendering(const std::vector<std::any> &params) override;

        void afterRendering(const std::vector<std::any> &params) override;
    };
}

#endif //DEMONENGINE_ENTITY_HPP
