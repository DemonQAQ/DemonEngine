//
// Created by Demon on 2024/3/27.
//

#ifndef DEMONENGINE_EFFECT_HPP
#define DEMONENGINE_EFFECT_HPP

#include <core/base/interface/IMetaAccessor.hpp>
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"
#include "core/io/interface/ISerializable.hpp"

using namespace base;

namespace assets::scene
{

    class Effect
            : implements Object,
              implements INameable,
              implements ITransformableUpdate,
              implements io::ISerializable,
              implements IMetaAccessor
    {
    private:
        std::string name;
    public:
        explicit Effect(const std::shared_ptr<base::UUID> &existingUuid,
                        bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                        std::string name = "Effect Entity");

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        [[nodiscard]] std::string Serialize() const override;

        void Deserialize(const std::string &data) override;

        void beforeRendering(const std::vector<std::any> &params) override;

        void afterRendering(const std::vector<std::any> &params) override;
    };

} // assets

#endif //DEMONENGINE_EFFECT_HPP
