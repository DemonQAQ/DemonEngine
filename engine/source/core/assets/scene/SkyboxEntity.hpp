//
// Created by Demon on 2024/3/27.
//

#ifndef DEMONENGINE_SKYBOXENTITY_HPP
#define DEMONENGINE_SKYBOXENTITY_HPP

#include <core/base/interface/IMetaAccessor.hpp>
#include <core/base/interface/ISkyBox.hpp>
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"
#include "core/io/interface/ISerializable.hpp"

using namespace base;

namespace assets::scene
{
    class SkyboxEntity : implements Object,
                         implements INameable,
                         implements io::ISerializable,
                         implements IMetaAccessor,
                         implements IRenderUpdatable
    {
    private:
        std::string name;
        std::shared_ptr<ISkyBox> skybox;
    public:
        explicit SkyboxEntity(const std::shared_ptr<base::UUID> &existingUuid,
                              bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                              const std::shared_ptr<ISkyBox> &skybox = nullptr,
                              std::string name = "SkyboxEntity");

        ~SkyboxEntity() override;

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        [[nodiscard]] std::string serialize() const override;

        void deserialize(const std::string &data) override;

        std::shared_ptr<ISkyBox> &getSkyBox();

        void beforeRendering(const std::vector <std::any> &params) override;

        void afterRendering(const std::vector <std::any> &params) override;
    };
}


#endif //DEMONENGINE_SKYBOXENTITY_HPP
