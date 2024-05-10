//
// Created by Demon on 2024/5/11.
//

#ifndef DEMONENGINE_CYLINDER_HPP
#define DEMONENGINE_CYLINDER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/scene/interfaace/ICommonShape.hpp"
#include "core/assets/manager/loader/AssimpLoader.hpp"

namespace base
{
    class Cylinder : implements ICommonShape
    {
    private:
        static bool isInit;
        static std::shared_ptr<base::Model> MODEL_TEMPLATE;
    public:
        Cylinder(const std::shared_ptr<base::UUID> &existingUuid, bool init, std::shared_ptr<io::YamlConfiguration> &yml)
                : ICommonShape(existingUuid, init, "Cylinder", yml)
        {

        }

        static void init()
        {
            if (isInit)return;
            MODEL_TEMPLATE = assets::AssimpLoader::loadModel("/model/cylinder.obj", true);
            isInit = MODEL_TEMPLATE != nullptr;
        }

        static void finalize()
        {
            MODEL_TEMPLATE = nullptr;
            isInit = false;
        }
    };
}

#endif //DEMONENGINE_CYLINDER_HPP
