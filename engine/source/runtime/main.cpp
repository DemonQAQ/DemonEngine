//
// Created by Demon on 2023/12/6.
//

#include "base/RuntimeApplication.hpp"
#include <iostream>
#include <core/assets/scene/Entity.hpp>
#include "core/assets/manager/AssetsMainManager.hpp"
#include "core/assets/manager/ModelManager.hpp"
#include "core/assets/manager/ShaderManager.hpp"
#include "core/io/FileSystem.hpp"

using namespace std;

runtime::RuntimeApplication runtimeApp;

int main()
{
    runtimeApp.initialize();

    auto scene = std::make_shared<assets::scene::Scene>();

    auto modelManager = assets::AssetsMainManager::getManagerAs<assets::ModelManager>(assets::AssetType::MODEL);
    auto shaderManager = assets::AssetsMainManager::getManagerAs<assets::ShaderManager>(assets::AssetType::SHADER);

    std::string modelPath = FileSystem::combinePaths(SOURCE_ROOT_PATH, "/package0/model/untitled.obj");
    auto uuid = modelManager->loadResource({modelPath});
    auto modelOpt = modelManager->getResourceByUuid(uuid.value());

    std::string fshPath = FileSystem::combinePaths(SOURCE_ROOT_PATH, "/package0/shader/fsh/Color.fsh");
    std::string vshPath = FileSystem::combinePaths(SOURCE_ROOT_PATH, "/package0/shader/fsh/Color.vsh");
    uuid = shaderManager->loadResource({fshPath, vshPath});

    std::shared_ptr<Model> model = modelOpt.value();
    model->bindShader(uuid.value());

    std::shared_ptr<assets::scene::Entity> entity = std::make_shared<assets::scene::Entity>();
    entity->addModel(modelOpt.value());
    scene->addChild(entity);

    runtimeApp.loadScene(scene);
    runtimeApp.start();

    return 0;
}