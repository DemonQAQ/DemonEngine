﻿//
// Created by Demon on 2024/3/26.
//

#ifndef DEMONENGINE_PROGRAMMABLEPIPELINE_HPP
#define DEMONENGINE_PROGRAMMABLEPIPELINE_HPP

#include <core/base/interface/ISkyBox.hpp>
#include "core/base/interface/Interface.hpp"
#include "RenderPipeline.hpp"
#include "core/assets/manager/data/ShaderManager.hpp"
#include "core/assets/manager/data/MaterialsManager.hpp"

namespace render
{
    class ProgrammablePipeline : implements RenderPipeline
    {
    private:
        std::shared_ptr<assets::ShaderManager> shaderManager;
        std::shared_ptr<assets::MaterialsManager> materialsManager;
        std::shared_ptr<base::ISkyBox> skyBox;
        //渲染前会将这两个list中的内容打包至drawcall的list中，打包时没有shader的实体使用管线默认的shader
        std::vector<std::shared_ptr<base::IRenderable>> opaqueRenderableEntityList;
        std::vector<std::shared_ptr<base::IRenderable>> transparentRenderableEntityList;

        std::vector<std::shared_ptr<DrawCall>> opaqueDrawCalls;
        std::vector<std::shared_ptr<DrawCall>> transparentDrawCalls;
        std::shared_ptr<base::Shader> usingShader;
        //后处理的shader列表
    public:
        explicit ProgrammablePipeline(std::shared_ptr<GraphApi> &graphApi_);

        void render() override;

        void submitEntity(std::shared_ptr<base::IRenderable> object, RenderType renderType) override;

        void submitDrawCall(std::shared_ptr<DrawCall> drawCall, RenderType renderType) override;

        void bindShader(std::shared_ptr<base::Shader> shader) override;

        void unbindShader() override;

        void prepare() override;

        void clear() override;

    private:
        void drawSkyBox();

        void executeDrawCalls(const std::vector<std::shared_ptr<DrawCall>> &drawCallList);

        void setupRenderState(const std::shared_ptr<DrawCall> &drawCall);

        void sortAndExecuteDrawCalls();

        std::vector<std::shared_ptr<DrawCall>> createAndSubmitDrawCalls(const std::shared_ptr<base::IRenderable>& renderable);
    };
}

#endif //DEMONENGINE_PROGRAMMABLEPIPELINE_HPP
