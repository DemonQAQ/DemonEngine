//
// Created by Demon on 2024/3/26.
//

#include <core/base/common/platform/Application.hpp>
#include <core/assets/manager/AssetsDataMainManager.hpp>
#include <core/assets/manager/data/ShaderManager.hpp>
#include <core/assets/manager/data/MaterialsManager.hpp>
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
    graphApi->clear(32 / 255.0f, 32 / 255.0f, 32 / 255.0f, 255 / 255.0f);
    opaqueRenderableEntityList.clear();
    transparentRenderableEntityList.clear();
    opaqueDrawCalls.clear();
    transparentDrawCalls.clear();
}

GLuint setupSimpleShader() {
    const char* vertexShaderSource = R"glsl(
        #version 410 core
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;
        out vec3 vertexColor;
        void main() {
            gl_Position = vec4(position, 1.0);
            vertexColor = color;
        }
    )glsl";

    const char* fragmentShaderSource = R"glsl(
        #version 410 core
        in vec3 vertexColor;
        out vec4 FragColor;
        void main() {
            FragColor = vec4(vertexColor, 1.0);
        }
    )glsl";

    // Compile shaders and create a shader program
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // Check for shader compile errors...

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // Check for shader compile errors...

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors...

    GLint success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void ProgrammablePipeline::render()
{
    std::cerr << "render" << std::endl;

    // Setup the shader program
    GLuint shaderProgram = setupSimpleShader();

    // Seed random number generator for color values
    srand(static_cast<unsigned int>(time(nullptr)));

    // Define vertices for a triangle with random colors
    float vertices[] = {
            // Positions        // Colors
            -0.5f, -0.5f, 0.0f,  rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f,  // Vertex 1
            0.5f, -0.5f, 0.0f,  rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f,  // Vertex 2
            0.0f,  0.5f, 0.0f,  rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f   // Vertex 3
    };

    // Generate and bind VAO and VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Use the shader program and draw the triangle
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Clean up
    glBindVertexArray(0);
    glUseProgram(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
//    prepare();
//    executeDrawCalls(opaqueDrawCalls);
//    //todo 其他渲染任务
//    executeDrawCalls(transparentDrawCalls);
//    //todo 后处理任务
//    clear();
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
    auto shaderManager = assets::AssetsDataMainManager::getManagerAs<assets::ShaderManager>(assets::AssetType::SHADER);
    auto materialsManager = assets::AssetsDataMainManager::getManagerAs<assets::MaterialsManager>(
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



