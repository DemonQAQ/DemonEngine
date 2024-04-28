//
// Created by Demon on 2024/3/26.
//

#include <core/assets/manager/AssetsDataMainManager.hpp>
#include <core/assets/manager/data/ShaderManager.hpp>
#include <core/assets/manager/data/MaterialsManager.hpp>
#include "ProgrammablePipeline.hpp"
#include "OpenglDrawCall.hpp"
#include <future>
#include <vector>

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
}


void render::ProgrammablePipeline::executeDrawCalls(const std::vector<std::shared_ptr<DrawCall>> &drawCallList)
{
    int i = 0;
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
    auto shaderManager = assets::AssetsDataMainManager::getManagerAs<assets::ShaderManager>(assets::AssetType::SHADER);
    auto materialsManager = assets::AssetsDataMainManager::getManagerAs<assets::MaterialsManager>(
            assets::AssetType::MATERIALS);

    std::vector<std::future<std::vector<std::shared_ptr<DrawCall>>>> futures;

    for (auto &renderable: opaqueRenderableEntityList)
    {
        futures.push_back(std::async(std::launch::async,
                                     &ProgrammablePipeline::createAndSubmitDrawCalls, this,
                                     renderable, std::ref(shaderManager), std::ref(materialsManager)));
    }

    for (auto &future: futures)
    {
        auto drawCalls = future.get();
        opaqueDrawCalls.insert(opaqueDrawCalls.end(), drawCalls.begin(), drawCalls.end());
    }

    futures.clear();
    for (auto &renderable: transparentRenderableEntityList)
    {
        futures.push_back(std::async(std::launch::async,
                                     &ProgrammablePipeline::createAndSubmitDrawCalls, this,
                                     renderable, std::ref(shaderManager), std::ref(materialsManager)));
    }

    for (auto &future: futures)
    {
        auto drawCalls = future.get();
        transparentDrawCalls.insert(transparentDrawCalls.end(), drawCalls.begin(), drawCalls.end());
    }

    sortAndExecuteDrawCalls();
}

void ProgrammablePipeline::sortAndExecuteDrawCalls()
{
    auto sortFunc = [](const std::shared_ptr<DrawCall> &a, const std::shared_ptr<DrawCall> &b) -> bool
    {
        return a->shader->getUUID() < b->shader->getUUID() ||
               (a->shader->getUUID() == b->shader->getUUID() && a->material->getUUID() < b->material->getUUID());
    };

    std::sort(opaqueDrawCalls.begin(), opaqueDrawCalls.end(), sortFunc);
    std::sort(transparentDrawCalls.begin(), transparentDrawCalls.end(), sortFunc);

    for (const auto &drawCall: opaqueDrawCalls)
        drawCall->preExecute();
    for (const auto &drawCall: transparentDrawCalls)
        drawCall->preExecute();

    graphApi->clear();
}

std::vector<std::shared_ptr<DrawCall>>
ProgrammablePipeline::createAndSubmitDrawCalls(const std::shared_ptr<base::IRenderable> &renderable,
                                               std::shared_ptr<assets::ShaderManager> &shaderManager,
                                               std::shared_ptr<assets::MaterialsManager> &materialsManager)
{
    std::vector<base::RenderData> renderDataList;
    renderable->getRenderData(renderDataList);
    std::vector<std::shared_ptr<DrawCall>> drawCalls;

    std::map<std::pair<std::shared_ptr<base::Shader>, std::shared_ptr<base::Material>>, std::shared_ptr<OpenglDrawCall>> drawCallsMap;

    for (auto &data: renderDataList)
    {
        std::shared_ptr<base::Shader> shader;
        std::shared_ptr<base::Material> material;

        if (data.useShader)
        {
            auto shaderOpt = shaderManager->getResourceByUuid(data.useShader);
            if (shaderOpt) shader = shaderOpt.value();
        }

        if (data.material)
        {
            auto materialOpt = materialsManager->getResourceByUuid(data.material);
            if (materialOpt) material = materialOpt.value();
        }

        auto key = std::make_pair(shader, material);

        if (!drawCallsMap.count(key))
        {
            auto newDrawCall = std::make_shared<OpenglDrawCall>();
            newDrawCall->shader = shader;
            newDrawCall->material = material;
            drawCallsMap[key] = newDrawCall;
        }

        render::DrawCallRenderData drawCallRenderData;
        drawCallRenderData.modelMatrix = data.globalTransform.toMatrix();
        drawCallRenderData.vertices = data.vertices;
        drawCallRenderData.indices = data.indices;
        drawCallsMap[key]->data.push_back(drawCallRenderData);
    }

    drawCalls.reserve(drawCallsMap.size());
    for (auto &entry: drawCallsMap)
    {
        drawCalls.push_back(entry.second);
    }

    return drawCalls;
}

