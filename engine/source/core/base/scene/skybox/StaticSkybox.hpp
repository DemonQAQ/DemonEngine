//
// Created by Demon on 2024/5/11.
//

#ifndef DEMONENGINE_STATICSKYBOX_HPP
#define DEMONENGINE_STATICSKYBOX_HPP

#include "core/base/interface/ISkyBox.hpp"
#include "core/base/scene/shape/Cube.hpp"

namespace base
{
    class StaticSkybox : ISkyBox
    {
    private:
        std::shared_ptr<Cube> cube;
        std::shared_ptr<Texture> texture;
    };

}

#endif //DEMONENGINE_STATICSKYBOX_HPP
