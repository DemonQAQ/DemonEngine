//
// Created by Demon on 2024/3/24.
//

#ifndef DEMONENGINE_OPENGLDRAWCALL_HPP
#define DEMONENGINE_OPENGLDRAWCALL_HPP


#include "core/base/interface/Interface.hpp"
#include "DrawCall.hpp"

namespace render
{
    class OpenglDrawCall : implements DrawCall
    {
    public:
        GLuint VAO = 0, VBO = 0, EBO = 0;
        GLsizei vertexCount = 0, indexCount = 0;

        void preExecute() override;

        void afterExecute() override;
    };
}

#endif //DEMONENGINE_OPENGLDRAWCALL_HPP
