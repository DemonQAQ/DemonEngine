//
// Created by Demon on 2024/3/28.
//

#ifndef DEMONENGINE_CAMERAENTITY_HPP
#define DEMONENGINE_CAMERAENTITY_HPP

#include <core/base/interface/IMetaAccessor.hpp>
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"
#include "core/base/render/Camera.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"
#include "core/io/interface/ISerializable.hpp"

using namespace base;

namespace assets::scene
{

    class CameraEntity
            : implements Object,
              implements INameable,
              implements ITransformableUpdate,
              implements io::ISerializable,
              implements IMetaAccessor
    {
    private:
        std::string name;
        Camera camera;
    public:
        CameraEntity(const std::shared_ptr<base::UUID> &existingUuid,
                     bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                     const base::Camera &camera_ = {}, std::string name = "Normal Camera");

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        [[nodiscard]]  std::string Serialize() const override;

        void Deserialize(const std::string &data) override;

        void beforeRendering(const std::vector<std::any> &params) override;

        void afterRendering(const std::vector<std::any> &params) override;

        void setCameraPosition(const glm::vec3 &position);

        void setCameraRotation(const glm::quat &rotation);

        void setCameraMovementSpeed(float speed);

        void setCameraMouseSensitivity(float sensitivity);

        void setCameraZoom(float zoom);

        [[nodiscard]] glm::vec3 getCameraPosition() const;

        [[nodiscard]] glm::quat getCameraRotation() const;

        [[nodiscard]] float getCameraMovementSpeed() const;

        [[nodiscard]] float getCameraMouseSensitivity() const;

        [[nodiscard]] float getCameraZoom() const;
    };

}

#endif //DEMONENGINE_CAMERAENTITY_HPP
