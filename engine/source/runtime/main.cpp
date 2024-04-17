//
// Created by Demon on 2023/12/6.
//

#include "base/RuntimeApplication.hpp"
#include <iostream>
#include <core/assets/scene/Entity.hpp>
#include <core/assets/manager/type/ConfigLoader.hpp>
#include <core/assets/manager/type/AssimpLoader.hpp>
#include <core/assets/manager/type/ShaderLoader.hpp>
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

    auto metaYml = assets::ConfigLoader::loadYml("/package0/scene/test.scene.meta", true);
    auto scene = std::make_shared<assets::scene::Scene>(base::UUIDManager::getUUID(utils::uuidUtil::getUUID(), false), true, metaYml);

    auto model = assets::AssimpLoader::loadModel("/package0/model/untitled.obj");

    auto shader = assets::ShaderLoader::loadShader("testShader", "/package0/shader/fsh/Color.fsh",
                                                   "/package0/shader/fsh/Color.vsh");
    model->bindShader(shader->getUUID());

    auto entityMetaYml = assets::ConfigLoader::loadYml("/package0/scene/test.entity.meta", true);
    std::shared_ptr<assets::scene::Entity> entity = std::make_shared<assets::scene::Entity>(base::UUIDManager::getUUID(utils::uuidUtil::getUUID(), false), true,
                                                                                            entityMetaYml,
                                                                                            "testEntity");
    entity->addModel(model);
    scene->addChild(entity);

    runtimeApp.loadScene(scene);
    runtimeApp.start();

    return 0;
}