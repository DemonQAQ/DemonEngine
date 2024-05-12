//
// Created by Demon on 2024/3/29.
//

#ifndef DEMONENGINE_SCENEFILE_HPP
#define DEMONENGINE_SCENEFILE_HPP

#include "core/base/interface/Interface.hpp"
#include "core/io/interface/IFile.hpp"
#include "core/io/interface/ISerializable.hpp"
#include "core/assets/scene/Scene.hpp"

namespace io
{
    class SceneFile : implements IFile, ISerializable
    {
        //todo 构造时先从路径读取文件，然后调用Serialize方法将数据存在实例中
        explicit SceneFile(const std::string &path);

        //todo 调用时将实例中的数据初始化为scene实例返回
        //assets::scene::Scene loadScene();

        void saveScene(const assets::scene::Scene &scene, const std::string &path);

        [[nodiscard]] std::string serialize() const override;

        void deserialize(const std::string &data) override;

        void load() override;

        void save() const override;

        bool isEmpty() const override;
    };
}

#endif //DEMONENGINE_SCENEFILE_HPP
