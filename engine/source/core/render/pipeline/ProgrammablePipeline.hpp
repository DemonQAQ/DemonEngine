//
// Created by Demon on 2024/3/26.
//

#ifndef DEMONENGINE_PROGRAMMABLEPIPELINE_HPP
#define DEMONENGINE_PROGRAMMABLEPIPELINE_HPP

#include "core/base/interface/Interface.hpp"
#include "RenderPipeline.hpp"

namespace render
{
    class ProgrammablePipeline : implements RenderPipeline
    {
    private:
        //渲染前会将这两个list中的内容打包至drawcall的list中，打包时没有shader的实体使用管线默认的shader
        std::vector<base::IRenderable *> opaqueRenderableEntityList;
        std::vector<base::IRenderable *> transparentRenderableEntityList;

        std::vector<DrawCall *> opaqueDrawCalls;
        std::vector<DrawCall *> transparentDrawCalls;
        std::shared_ptr<base::Shader> usingShader;
        //后处理的shader列表
    public:
        void render() override
        {
            //todo 准备工作
            //todo 渲染opaqueDrawCalls
            //todo 其他渲染任务
            //todo 渲染transparentDrawCalls
            //todo 后处理任务
            //todo 输出到屏幕
        }

        void submitEntity(base::IRenderable *object, RenderType renderType) override;

        void submitDrawCall(DrawCall *drawCall, RenderType renderType) override;

        void bindShader(base::Shader *shader) override;

        void unbindShader(base::Shader *shader) override;
    };
}

#endif //DEMONENGINE_PROGRAMMABLEPIPELINE_HPP
