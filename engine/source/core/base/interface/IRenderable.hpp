//
// Created by Demon on 2024/3/2.
//

#ifndef DEMONENGINE_IRENDERABLE_HPP
#define DEMONENGINE_IRENDERABLE_HPP

#include "core/base/common/Transform.hpp"

namespace base
{
    struct RenderData
    {
        //todo 根据需要添加熟数据结构
    };

    class IRenderable
    {
    public:
        virtual RenderData getRenderData(Transform transform) = 0;
    };

}

#endif //DEMONENGINE_IRENDERABLE_HPP
