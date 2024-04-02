//
// Created by Demon on 2024/3/26.
//

#include <core/base/common/platform/Application.hpp>
#include <core/assets/manager/AssetsMainManager.hpp>
#include <core/assets/manager/ShaderManager.hpp>
#include <core/assets/manager/MaterialsManager.hpp>
#include "ProgrammablePipeline.hpp"
#include "OpenglDrawCall.hpp"

using namespace render;

ProgrammablePipeline::ProgrammablePipeline(std::shared_ptr<GraphApi> &graphApi_) : RenderPipeline(graphApi_)
{

}


void ProgrammablePipeline::submitEntity(std::shared_ptr<base::IRenderable> object, render::RenderType renderType)
{
    auto &renderQueue = (renderType == RenderType::OPAQUE) ? opaqueRenderableEntityList
                                                           : transparentRenderableEntityList;

    if (std::find(renderQueue.begin(), renderQueue.end(), object) == renderQueue.end())
    {
        renderQueue.push_back(object);
    }
}

void ProgrammablePipeline::submitDrawCall(std::shared_ptr<DrawCall> drawCall, RenderType renderType)
{
    auto &drawCallQueue = (renderType == RenderType::OPAQUE) ? opaqueDrawCalls : transparentDrawCalls;

    if (std::find(drawCallQueue.begin(), drawCallQueue.end(), drawCall) == drawCallQueue.end())
    {
        drawCallQueue.push_back(drawCall);
    }
}

void ProgrammablePipeline::bindShader(std::shared_ptr<base::Shader> shader)
{
    graphApi->useShader(shader);
}

void ProgrammablePipeline::unbindShader()
{
    graphApi->releaseShader();
}

void ProgrammablePipeline::clear()
{
    for (const auto &drawCall: opaqueDrawCalls)drawCall->afterExecute();
    for (const auto &drawCall: transparentDrawCalls)drawCall->afterExecute();
    graphApi->clear(255, 255, 255, 255);
    opaqueRenderableEntityList.clear();
    transparentRenderableEntityList.clear();
    opaqueDrawCalls.clear();
    transparentDrawCalls.clear();
}

void ProgrammablePipeline::render()
{
    prepare();
    executeDrawCalls(opaqueDrawCalls);
    //todo 其他渲染任务
    executeDrawCalls(transparentDrawCalls);
    //todo 后处理任务
    clear();
    //todo 输出到屏幕
}


void render::ProgrammablePipeline::executeDrawCalls(const std::vector<std::shared_ptr<DrawCall>> &drawCallList)
{
    for (const auto &drawCall: drawCallList)
    {
        if (graphApi->getUsingShader() != drawCall->shader) graphApi->useShader(drawCall->shader);
        setupRenderState(drawCall);
        graphApi->executeDrawCall(drawCall);
    }
}

void ProgrammablePipeline::setupRenderState(const std::shared_ptr<DrawCall> &drawCall)
{
    graphApi->setBlendMode(drawCall->blendMode);
    graphApi->enableDepthTest(drawCall->depthFunction != DepthFunction::ALWAYS);
    graphApi->setDepthFunction(drawCall->depthFunction);
    graphApi->enableCulling(drawCall->cullFace != CullFace::FRONT_AND_BACK);
    graphApi->setCullFace(drawCall->cullFace);
}

void ProgrammablePipeline::prepare()
{
    auto shaderManager = assets::AssetsMainManager::getManagerAs<assets::ShaderManager>(assets::AssetType::SHADER);
    auto materialsManager = assets::AssetsMainManager::getManagerAs<assets::MaterialsManager>(
            assets::AssetType::MATERIALS);

    auto createAndSubmitDrawCalls = [&](std::vector<std::shared_ptr<base::IRenderable>> &renderableList,
                                        std::vector<std::shared_ptr<DrawCall>> &drawCallList)
    {
        for (const auto &renderable: renderableList)
        {
            std::vector<base::RenderData> renderDataList;
            renderable->getRenderData(renderDataList);

            for (auto &data: renderDataList)
            {
                auto newDrawCall = std::make_shared<OpenglDrawCall>();

                // 使用uuid获取对应的shader的shared_ptr
                if (data.useShader)
                {
                    auto useShader = shaderManager->getResourceByUuid(data.useShader);
                    if (useShader)
                    {
                        newDrawCall->shader = useShader.value();
                    }
                }

                // 使用uuid获取对应的material的shared_ptr
                if (data.material)
                {
                    auto material = materialsManager->getResourceByUuid(data.material);
                    if (material)
                    {
                        newDrawCall->material = material.value();
                    }
                }

                // 将base::RenderData 的数据加载到DrawCallRenderData中
                for (const auto &renderDataItem: renderDataList)
                {
                    render::DrawCallRenderData drawCallRenderData;
                    drawCallRenderData.modelMatrix = renderDataItem.globalTransform.toMatrix();
                    drawCallRenderData.vertices = renderDataItem.vertices;
                    drawCallRenderData.indices = renderDataItem.indices;
                    newDrawCall->data.push_back(drawCallRenderData);
                }

                drawCallList.push_back(newDrawCall);
            }
        }
    };

    createAndSubmitDrawCalls(opaqueRenderableEntityList, opaqueDrawCalls);
    createAndSubmitDrawCalls(transparentRenderableEntityList, transparentDrawCalls);

    // 排序函数，根据shader和material排序
    auto sortFunc = [](const std::shared_ptr<DrawCall> &a, const std::shared_ptr<DrawCall> &b) -> bool
    {
        if (a->shader && b->shader && a->shader->getUUID() == b->shader->getUUID())
        {
            if (a->material && b->material)return a->material->getUUID() < b->material->getUUID();
        }
        return a->shader && b->shader && a->shader->getUUID() < b->shader->getUUID();
    };

    // 对DrawCalls列表进行排序
    std::sort(opaqueDrawCalls.begin(), opaqueDrawCalls.end(), sortFunc);
    std::sort(transparentDrawCalls.begin(), transparentDrawCalls.end(), sortFunc);

    for (const auto &drawCall: opaqueDrawCalls)drawCall->preExecute();
    for (const auto &drawCall: transparentDrawCalls)drawCall->preExecute();
}



