//
// Created by Demon on 2024/5/11.
//

#ifndef DEMONENGINE_CENTRUM_HPP
#define DEMONENGINE_CENTRUM_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/scene/interfaace/ICommonShape.hpp"
#include "core/assets/manager/loader/AssimpLoader.hpp"

namespace base
{
    class Centrum : implements ICommonShape
    {
    private:
        static bool isInit;
        static std::shared_ptr<base::Model> MODEL_TEMPLATE;
    public:
        Centrum(const std::shared_ptr<base::UUID> &existingUuid, bool init, std::shared_ptr<io::YamlConfiguration> &yml)
                : ICommonShape(existingUuid, init, "Centrum", yml)
        {
            if (isInit)meshes = MODEL_TEMPLATE->getRootNode()->meshes;
        }

        static void init()
        {
            if (isInit)return;
            MODEL_TEMPLATE = assets::AssimpLoader::loadModel("/model/centrum.obj", true);
            isInit = MODEL_TEMPLATE != nullptr;
        }

        static void finalize()
        {
            MODEL_TEMPLATE = nullptr;
            isInit = false;
        }
    };

} // scene

#endif //DEMONENGINE_CENTRUM_HPP
