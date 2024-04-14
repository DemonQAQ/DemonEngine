//
// Created by Demon on 2023/12/6.
//

#include "base/RuntimeApplication.hpp"
#include <iostream>
#include <core/assets/scene/Entity.hpp>
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

    auto configManagerOpt = assets::AssetsDataMainManager::getManager(assets::AssetType::CONFIG);
    auto configManager = std::dynamic_pointer_cast<assets::ConfigManager>(configManagerOpt.value());

    auto metaUuid = configManager->loadData({"/package0/scene/test.scene.meta"});
    if (!metaUuid.has_value())
    {
        std::cerr << "Failed to load metadata from: " << std::endl;
    }

    auto metaFileOpt = configManager->getResourceByUuid(metaUuid.value());
    if (!metaFileOpt.has_value())
    {
        std::cerr << "Failed to find metadata file with UUID: " << metaUuid.value()->toString() << std::endl;
    }

    auto metaFile = std::dynamic_pointer_cast<io::YamlConfiguration>(metaFileOpt.value());
    if (!metaFile)
    {
        std::cerr << "Metadata file is not a YAML configuration." << std::endl;
    }

    auto scene = std::make_shared<assets::scene::Scene>(uuidStr, true, metaFile);

    auto modelManager = assets::AssetsDataMainManager::getManagerAs<assets::ModelManager>(assets::AssetType::MODEL);
    auto shaderManager = assets::AssetsDataMainManager::getManagerAs<assets::ShaderManager>(assets::AssetType::SHADER);
    auto textureManager = assets::AssetsDataMainManager::getManagerAs<assets::ShaderManager>(assets::AssetType::TEXTURE);

    auto uuid = modelManager->loadData({"/package0/model/untitled.obj"});
    auto modelOpt = modelManager->getResourceByUuid(uuid.value());

    textureManager->loadData({"/package0/model/mc3.png", TextureType::NORMAL});
    textureManager->loadData({"/package0/model/toon.png", TextureType::NORMAL});
    textureManager->loadData({"/package0/model/头发.png", TextureType::NORMAL});
    textureManager->loadData({"/package0/model/披风.png", TextureType::NORMAL});
    textureManager->loadData({"/package0/model/披风2.png", TextureType::NORMAL});
    textureManager->loadData({"/package0/model/脸.png", TextureType::NORMAL});
    textureManager->loadData({"/package0/model/衣服.png", TextureType::NORMAL});
    textureManager->loadData({"/package0/model/表情.png", TextureType::NORMAL});

    uuid = shaderManager->loadData(
            {"testShader", "/package0/shader/fsh/Color.fsh", "/package0/shader/fsh/Color.vsh"});

    std::shared_ptr<Model> model = modelOpt.value();
    model->bindShader(uuid.value());

    std::shared_ptr<assets::scene::Entity> entity = std::make_shared<assets::scene::Entity>();
    entity->addModel(modelOpt.value());
    scene->addChild(entity);

    runtimeApp.loadScene(scene);
    runtimeApp.start();

    return 0;
}