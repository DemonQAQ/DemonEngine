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

    class Scene : implements io::IFile, INameable, Updatable
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
        void update() override;

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        void load(const std::string &path) override;

        void save(const std::string &path) const override;

        //todo 为场景树下的某个节点增加子节点

        //todo 查询场景树下的某个节点

        //todo 更新场景树下的某个节点

        //todo 删除场景树下的某个节点

        [[nodiscard]] const std::shared_ptr<Skybox> &getSkybox() const;

        void setSkybox(const std::shared_ptr<Skybox> &skybox);

        [[nodiscard]] const std::shared_ptr<SceneGroup> &getRoot() const;

        void setRoot(const std::shared_ptr<SceneGroup> &root);

        [[nodiscard]] const std::shared_ptr<LightEntity> &getEnvironmentLight() const;

        void setEnvironmentLight(const std::shared_ptr<LightEntity> &environmentLight);

        [[nodiscard]] const std::shared_ptr<CameraEntity> &getMainCameraEntity() const;

        void setMainCameraEntity(const std::shared_ptr<CameraEntity> &mainCameraEntity);

        [[nodiscard]] const std::vector<std::shared_ptr<CameraEntity>> &getCameraEntityList() const;

        void setCameraEntityList(const std::vector<std::shared_ptr<CameraEntity>> &cameraEntityList);

        //todo 添加摄像机
    };

}

#endif //DEMONENGINE_SCENE_HPP
