﻿//
// Created by Demon on 2024/5/11.
//

#ifndef DEMONENGINE_ANGULARSPHERE_HPP
#define DEMONENGINE_ANGULARSPHERE_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/manager/loader/AssimpLoader.hpp"
#include "core/base/scene/interfaace/ICommonShape.hpp"

namespace base
{
    class AngularSphere : implements ICommonShape
    {
    private:
        static bool isInit;
        static std::shared_ptr<base::Model> MODEL_TEMPLATE;
    public:
        AngularSphere(const std::shared_ptr<base::UUID> &existingUuid, bool init, std::shared_ptr<io::YamlConfiguration> &yml)
                : ICommonShape(existingUuid, init, "ANGULAR_SPHERE", yml)
        {
            if (isInit)meshes = MODEL_TEMPLATE->getRootNode()->meshes;
        }

        static void init()
        {
            if (isInit)return;
            MODEL_TEMPLATE = assets::AssimpLoader::loadModel("/model/angularSphere.obj", true);
            isInit = MODEL_TEMPLATE != nullptr;
        }

        static void finalize()
        {
            MODEL_TEMPLATE = nullptr;
            isInit = false;
        }
    };

} // scene

#endif //DEMONENGINE_ANGULARSPHERE_HPP
