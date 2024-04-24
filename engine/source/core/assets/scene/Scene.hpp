//
// Created by Demon on 2024/3/27.
//

#ifndef DEMONENGINE_SCENE_HPP
#define DEMONENGINE_SCENE_HPP

#include "Skybox.hpp"
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"
#include "SceneGroup.hpp"
#include "LightEntity.hpp"
#include "CameraEntity.hpp"
#include "core/base/interface/IUpdatable.hpp"
#include "core/io/interface/IFile.hpp"

using namespace base;

namespace assets::scene
{

    class Scene
            : implements Object,
              implements INameable,
              implements Updatable,
              implements io::ISerializable,
              implements IRenderUpdatable,
              implements IMetaAccessor
    {
    private:
        std::string name;
        std::shared_ptr<Skybox> skybox;
        std::shared_ptr<SceneGroup> root;
        std::shared_ptr<LightEntity> environmentLight;
        std::shared_ptr<CameraEntity> mainCameraEntity;
        std::vector<std::shared_ptr<CameraEntity>> cameraEntityList;
        //todo 物理系统参数，待实现

    public:
        explicit Scene(const std::shared_ptr<base::UUID> &existingUuid,
                       bool init, std::shared_ptr<io::YamlConfiguration> &yml,
                       std::string name = "New Scene");

        void update() override;

        void beforeRendering(const std::vector<std::any> &params) override;

        void afterRendering(const std::vector<std::any> &params) override;

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string Serialize() const override;

        void Deserialize(const std::string &data) override;

        [[nodiscard]] std::string getName() const override;

        void addChild(const std::shared_ptr<Object> &child);

        bool addChildToNode(const std::shared_ptr<base::UUID> &parentUuid, const std::shared_ptr<Object> &child);

        [[nodiscard]] std::shared_ptr<Object>
        findNodeByUUID(const std::shared_ptr<SceneGroup> &node, const std::shared_ptr<base::UUID> &uuid) const;

        bool updateNode(const std::shared_ptr<base::UUID> &uuid, const std::shared_ptr<Object> &newNode);

        bool updateNodeRecursive(const std::shared_ptr<SceneGroup> &node, const std::shared_ptr<base::UUID> &uuid,
                                 const std::shared_ptr<Object> &newNode);

        bool removeChildFromNode(const std::shared_ptr<base::UUID> &uuid);

        bool
        removeChildFromNodeRecursive(const std::shared_ptr<SceneGroup> &node, const std::shared_ptr<base::UUID> &uuid);

        [[nodiscard]] const std::shared_ptr<Skybox> &getSkybox() const;

        void setSkybox(const std::shared_ptr<Skybox> &skybox_);

        [[nodiscard]] const std::shared_ptr<SceneGroup> &getRoot() const;

        void setRoot(const std::shared_ptr<SceneGroup> &root_);

        [[nodiscard]] const std::shared_ptr<LightEntity> &getEnvironmentLight() const;

        void setEnvironmentLight(const std::shared_ptr<LightEntity> &environmentLight_);

        [[nodiscard]] const std::shared_ptr<CameraEntity> &getMainCameraEntity() const;

        void setMainCameraEntity(const std::shared_ptr<CameraEntity> &mainCameraEntity_);

        [[nodiscard]] const std::vector<std::shared_ptr<CameraEntity>> &getCameraEntityList() const;

        void setCameraEntityList(const std::vector<std::shared_ptr<CameraEntity>> &cameraEntityList_);

        void addCameraEntity(const std::shared_ptr<CameraEntity> &cameraEntity);

    private:
        void traverseAndUpdate(const std::shared_ptr<Object> &node, const std::vector<std::any> &params);

        void updateNode(const std::shared_ptr<Object> &child);
    };

}

#endif //DEMONENGINE_SCENE_HPP
