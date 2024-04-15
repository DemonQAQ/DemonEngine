//
// Created by Demon on 2023/12/6.
//

#include "base/RuntimeApplication.hpp"
#include <iostream>
#include <core/assets/scene/Entity.hpp>
#include <core/assets/manager/type/ConfigLoader.hpp>
#include "core/assets/manager/AssetsDataMainManager.hpp"
#include "core/assets/manager/data/ModelManager.hpp"
#include "core/assets/manager/data/ShaderManager.hpp"
#include "core/assets/manager/data/ConfigManager.hpp"
#include "core/assets/manager/data/TextureManager.hpp"
#include "core/io/FileSystem.hpp"

using namespace std;

runtime::RuntimeApplication runtimeApp;

int main()
{
    runtimeApp.initialize();

    std::string uuidStr = utils::uuidUtil::getUUID("test");
    auto sceneUuid = UUIDManager::getUUID(uuidStr);

    auto metaYml = assets::ConfigLoader::loadYml("/package0/scene/test.scene.meta",true);
    auto scene = std::make_shared<assets::scene::Scene>(uuidStr, true, metaYml);

//    auto model = assets::AssimpLoader::loadModel("/package0/model/untitled.obj");
//
//
//    uuid = shaderManager->loadData(
//            {"testShader", "/package0/shader/fsh/Color.fsh", "/package0/shader/fsh/Color.vsh"});
//
//    std::shared_ptr<Model> model = modelOpt.value();
//    model->bindShader(uuid.value());
//
//    std::shared_ptr<assets::scene::Entity> entity = std::make_shared<assets::scene::Entity>();
//    entity->addModel(modelOpt.value());
//    scene->addChild(entity);

    runtimeApp.loadScene(scene);
    runtimeApp.start();

    return 0;
}