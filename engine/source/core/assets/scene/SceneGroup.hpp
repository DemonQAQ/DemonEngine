//
// Created by Demon on 2024/3/26.
//

#ifndef DEMONENGINE_SCENEGROUP_HPP
#define DEMONENGINE_SCENEGROUP_HPP

#include <core/base/interface/IMetaAccessor.hpp>
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"
#include "core/io/interface/ISerializable.hpp"

using namespace base;

namespace assets::scene
{

    class SceneGroup
            : implements Object,
              implements INameable,
              implements ITransformableUpdate,
              implements io::ISerializable,
              implements IMetaAccessor
    {
    private:
        std::string name;
        std::vector<std::shared_ptr<Object>> childrenList;
        std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<Object>, UUIDHash, UUIDEqual> childrenMap;
    public:
        explicit SceneGroup(const std::shared_ptr<base::UUID> &existingUuid,
                            bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                            std::string name = "Group");

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        [[nodiscard]] std::string serialize() const override;

        void deserialize(const std::string &data) override;

        void beforeRendering(const std::vector<std::any> &params) override;

        void afterRendering(const std::vector<std::any> &params) override;

        void addChild(const std::shared_ptr<Object> &child);

        bool removeChild(const std::shared_ptr<Object> &child);

        bool removeChildByUUID(const std::shared_ptr<base::UUID> &uuid);

        std::shared_ptr<base::Object> findChildByUUID(const std::shared_ptr<base::UUID> &uuid);

        bool updateChildByUUID(const std::shared_ptr<base::UUID> &uuid, const std::shared_ptr<base::Object> &newChild);

        [[nodiscard]] const std::vector<std::shared_ptr<Object>> &getChildren() const;
    };

}

#endif //DEMONENGINE_SCENEGROUP_HPP
