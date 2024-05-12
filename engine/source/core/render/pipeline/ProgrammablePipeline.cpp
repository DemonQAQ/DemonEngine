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
#include <core/render/manager/RenderManager.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace render;

ProgrammablePipeline::ProgrammablePipeline(std::shared_ptr<GraphApi> &graphApi_) : RenderPipeline(graphApi_)
{
    auto shaderManagerOpt = assets::AssetsDataMainManager::getManager(assets::AssetType::SHADER);
    if (!shaderManagerOpt.has_value()) return;

    auto shaderManager_ = std::dynamic_pointer_cast<assets::ShaderManager>(shaderManagerOpt.value());
    if (!shaderManager_) shaderManager = shaderManager_;
}


void ProgrammablePipeline::submitEntity(std::shared_ptr<base::IRenderable> object, render::RenderType renderType)
{
    std::shared_ptr<base::ISkyBox> skyBoxCandidate = std::dynamic_pointer_cast<base::ISkyBox>(object);
    if (skyBoxCandidate)
    {
        skyBox = skyBoxCandidate;
    }
    else
    {
        auto &renderQueue = (renderType == render::RenderType::OPAQUE) ? opaqueRenderableEntityList
                                                                       : transparentRenderableEntityList;
        if (std::find(renderQueue.begin(), renderQueue.end(), object) == renderQueue.end())
        {
            renderQueue.push_back(object);
        }
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
    drawSkyBox();
    executeDrawCalls(opaqueDrawCalls);
    executeDrawCalls(transparentDrawCalls);
    clear();
}


void render::ProgrammablePipeline::executeDrawCalls(const std::vector<std::shared_ptr<DrawCall>> &drawCallList)
{
    auto start_time = std::chrono::steady_clock::now();
    std::cerr << "onRender.pipeline.render.executeDrawCalls start" << std::endl;

    int i = 0;
    for (const auto &drawCall: drawCallList)
    {
        if (graphApi->getUsingShader() != drawCall->shader) graphApi->useShader(drawCall->shader);
        setupRenderState(drawCall);
        graphApi->executeDrawCall(drawCall);
    }

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> frame_duration = end_time - start_time;
    std::cerr << "onRender.pipeline.render.executeDrawCalls Duration = " << frame_duration.count() << " ms" << std::endl;
    std::cerr << "onRender.pipeline.render.executeDrawCalls end\n" << std::endl;
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
    auto start_time = std::chrono::steady_clock::now();
    std::cerr << "onRender.pipeline.render.prepare start" << std::endl;

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

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> frame_duration = end_time - start_time;
    std::cerr << "onRender.pipeline.render.prepare Duration = " << frame_duration.count() << " ms" << std::endl;
    std::cerr << "onRender.pipeline.render.prepare end\n" << std::endl;
}

void ProgrammablePipeline::sortAndExecuteDrawCalls()
{
    auto start_time = std::chrono::steady_clock::now();
    std::cerr << "onRender.pipeline.render.prepare.sortAndExecuteDrawCalls start" << std::endl;

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

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> frame_duration = end_time - start_time;
    std::cerr << "onRender.pipeline.render.prepare.sortAndExecuteDrawCalls Duration = " << frame_duration.count() << " ms" << std::endl;
    std::cerr << "onRender.pipeline.render.prepare.sortAndExecuteDrawCalls end\n" << std::endl;
}

std::vector<std::shared_ptr<DrawCall>>
ProgrammablePipeline::createAndSubmitDrawCalls(const std::shared_ptr<base::IRenderable> &renderable,
                                               std::shared_ptr<assets::ShaderManager> &shaderManager,
                                               std::shared_ptr<assets::MaterialsManager> &materialsManager)
{
    auto start_time = std::chrono::steady_clock::now();
    std::cerr << "onRender.pipeline.render.prepare.createAndSubmitDrawCalls start" << std::endl;

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

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> frame_duration = end_time - start_time;
    std::cerr << "onRender.pipeline.render.prepare.createAndSubmitDrawCalls Duration = " << frame_duration.count() << " ms" << std::endl;
    std::cerr << "onRender.pipeline.render.prepare.createAndSubmitDrawCalls end\n" << std::endl;

    return drawCalls;
}

void ProgrammablePipeline::drawSkyBox()
{
    if (!skyBox)
    {
        std::cerr << "Error: No skybox is set for rendering." << std::endl;
        return;
    }
    if (!shaderManager)
    {
        std::cerr << "Error: No shaderManager to get shader." << std::endl;
        return;
    }
    glDepthMask(GL_FALSE);

    auto shaderOpt = shaderManager->getResourceByUuid(skyBox->getShader());
    if (!shaderOpt)
    {
        std::cerr << "didn't get shader." << std::endl;
        return;
    }
    auto shader = shaderOpt.value();

    graphApi->useShader(shader);

    GLint vpLocation = glGetUniformLocation(shader->ID, "vp");
    if (vpLocation != -1)
    {
        glUniformMatrix4fv(vpLocation, 1, GL_FALSE, glm::value_ptr(render::vpMatrix));
    }

    // Bind the skybox texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox->getTexture()->id);

    // Generate and bind the VAO
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Fetch render data
    std::vector<RenderData> renderDataList;
    skyBox->getRenderData(renderDataList);
    const auto &renderData = renderDataList.front();  // Assuming the first one is what we need

    // Generate VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, renderData.vertices.size() * sizeof(Vertex), renderData.vertices.data(), GL_STATIC_DRAW);

    // Set vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);  // Assuming the position attribute is at offset 0

    // Generate EBO if indices are present
    if (!renderData.indices.empty())
    {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderData.indices.size() * sizeof(unsigned int), renderData.indices.data(), GL_STATIC_DRAW);
    }

    // Draw the skybox
    if (!renderData.indices.empty())
    {
        glDrawElements(GL_TRIANGLES, renderData.indices.size(), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, renderData.vertices.size());
    }

    // Cleanup
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (!renderData.indices.empty())
    {
        glDeleteBuffers(1, &EBO);
    }
    glDepthMask(GL_TRUE);
}

