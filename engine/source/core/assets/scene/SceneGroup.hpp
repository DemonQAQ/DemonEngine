//
// Created by Demon on 2024/3/26.
//

#ifndef DEMONENGINE_SCENEGROUP_HPP
#define DEMONENGINE_SCENEGROUP_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"

using namespace base;

namespace assets::scene
{

    class SceneGroup : implements Object, INameable, ITransformableUpdate
    {
    private:
        std::string name;
        std::vector<std::shared_ptr<Object>> childrenList;
        std::unordered_map<UUID, std::shared_ptr<Object>, std::hash<UUID>> childrenMap;
    public:
        explicit SceneGroup(std::string name = "Group");

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        void beforeRendering(const std::vector<std::any> &params) override;

        void afterRendering(const std::vector<std::any> &params) override;

        void addChild(const std::shared_ptr<Object> &child);

        bool removeChild(const std::shared_ptr<Object> &child);

        bool removeChildByUUID(const base::UUID &uuid);

        std::shared_ptr<base::Object> findChildByUUID(const base::UUID &uuid);

        bool updateChildByUUID(const base::UUID &uuid, const std::shared_ptr<base::Object> &newChild);

        [[nodiscard]] const std::vector<std::shared_ptr<Object>> &getChildren() const;
    };

}

#endif //DEMONENGINE_SCENEGROUP_HPP
