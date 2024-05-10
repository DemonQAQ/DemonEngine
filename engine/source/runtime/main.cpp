//
// Created by Demon on 2023/12/6.
//

#include "base/RuntimeApplication.hpp"
#include <iostream>
#include <core/assets/scene/Entity.hpp>
#include <core/assets/manager/loader/ConfigLoader.hpp>
#include <core/assets/manager/loader/AssimpLoader.hpp>
#include <core/assets/manager/loader/ShaderLoader.hpp>
#include <core/assets/manager/loader/ScriptLoader.hpp>

using namespace std;

runtime::RuntimeApplication runtimeApp;

int main()
{
    runtimeApp.initialize();

    auto metaYml = assets::ConfigLoader::loadYml("/package0/scene/test.scene.meta", true);
    auto scene = std::make_shared<assets::scene::Scene>(base::UUIDManager::getUUID(utils::uuidUtil::getUUID(), false),
                                                        true, metaYml);

    auto model = assets::AssimpLoader::loadModel("/package0/model/untitled.obj");

    //auto model1 = assets::AssimpLoader::loadModel("/package2/model/T1.obj");

    auto shader = assets::ShaderLoader::loadShader("testShader", "/package0/shader", "/package0/shader/vsh/Shader.vsh",
                                                   "/package0/shader/fsh/Shader.fsh");
    //auto script = assets::ScriptLoader::loadScript("/package0/script/test.cs");

    model->bindShader(shader->getUUID());
    model->updateAllMeshShader();

    //model1->bindShader(shader->getUUID());
    //model1->updateAllMeshShader();

    auto entityMetaYml = assets::ConfigLoader::loadYml("/package0/scene/test.entity.meta", true);
    std::shared_ptr<assets::scene::Entity> entity = std::make_shared<assets::scene::Entity>(
            base::UUIDManager::getUUID(utils::uuidUtil::getUUID(), false), true,
            entityMetaYml,
            "testEntity");
    entity->addModel(model);
    //entity->addModel(model1);
    scene->addChild(entity);

    runtimeApp.loadScene(scene);
    //runtimeApp.submitScript(std::dynamic_pointer_cast<script::IScriptEntity>(script), false);
    runtimeApp.start();

    return 0;
}