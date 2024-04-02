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
    protected:
        std::shared_ptr<GraphApi> graphApi = nullptr;
    public:
        RenderPipeline(std::shared_ptr<GraphApi> graphApi_)
        {
            graphApi = graphApi_;
        }

        virtual void render() = 0;

        virtual void submitEntity(std::shared_ptr<base::IRenderable> object, RenderType renderType) = 0;

        virtual void submitDrawCall(std::shared_ptr<DrawCall> drawCall, RenderType renderType) = 0;

        virtual void bindShader(std::shared_ptr<base::Shader> shader) = 0;

        virtual void unbindShader() = 0;

        virtual void prepare() = 0;

        virtual void clear() = 0;
    };
}

#endif //DEMONENGINE_RENDERPIPELINE_HPP
