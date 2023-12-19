//
// Created by Demon on 2023/12/12.
//

#ifndef DEMONENGINE_GRAPHAPI_HPP
#define DEMONENGINE_GRAPHAPI_HPP

#include "core/base/interface/Interface.hpp"

namespace base::render
{
    interface GraphApi
    {
    public:
        virtual void drawLine(float x1, float y1, float x2, float y2) = 0;
        virtual void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) = 0;
        virtual void drawRectangle(float x, float y, float width, float height) = 0;
        virtual void drawCircle(float centerX, float centerY, float radius) = 0;
    };
}

#endif //DEMONENGINE_GRAPHAPI_HPP
