//
// Created by Demon on 2023/12/12.
//

#ifndef DEMONENGINE_RENDERPIPELINE_HPP
#define DEMONENGINE_RENDERPIPELINE_HPP

#include "core/base/interface/IRenderable.hpp"
#include "DrawCall.hpp"

namespace render
{
    enum class PipelineType
    {
        FORWARD,
        DEFERRED,
        PROGRAMMABLE
    };

    enum class RenderType
    {
        OPAQUE,
        TRANSPARENT
    };

    class RenderPipeline
    {
    public:
        virtual void render() = 0;

        virtual void submitEntity(base::IRenderable *object, RenderType renderType) = 0;

        virtual void submitDrawCall(DrawCall *drawCall, RenderType renderType) = 0;

        virtual void bindShader(base::Shader *shader) = 0;

        virtual void unbindShader(base::Shader *shader) = 0;
    };
}

#endif //DEMONENGINE_RENDERPIPELINE_HPP
