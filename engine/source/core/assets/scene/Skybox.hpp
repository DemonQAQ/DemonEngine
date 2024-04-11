//
// Created by Demon on 2024/3/27.
//

#ifndef DEMONENGINE_SKYBOX_HPP
#define DEMONENGINE_SKYBOX_HPP

#include <core/base/interface/IMetaAccessor.hpp>
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"
#include "core/io/interface/ISerializable.hpp"

using namespace base;

namespace assets::scene
{
    class Skybox : implements Object, implements INameable, implements io::ISerializable, implements IMetaAccessor
    {
    private:
        std::string name;
    public:
        explicit Skybox(const std::string &uuidStr, bool isUUID, std::shared_ptr<io::YamlConfiguration> &yml,
                        std::string name = "Skybox");

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        [[nodiscard]] std::string Serialize() const override;

        void Deserialize(const std::string &data) override;
    };
}


#endif //DEMONENGINE_SKYBOX_HPP
