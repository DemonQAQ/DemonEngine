//
// Created by Demon on 2024/3/25.
//
#include "RenderManager.hpp"
#include "core/render/pipeline/ProgrammablePipeline.hpp"

namespace render
{
    glm::mat4 vpMatrix;
    glm::mat4 viewMatrix = glm::mat4(1.0f); // 视图矩阵
    glm::mat4 projectionMatrix = glm::mat4(1.0f); // 投影矩阵

    /**
     * params: RenderApiType,GLFWwindow*,PipelineType
     * */
    bool render::RenderManager::init(const std::vector<std::any> &params)
    {
        if (isInit())return true;
        if (params.empty()) return false;
        RenderApiType renderApi;
        try
        {
            renderApi = std::any_cast<RenderApiType>(params[0]);
        }
        catch (const std::bad_any_cast &e)
        {
            std::cerr << "Initialization failed: " << e.what() << std::endl;
            return false;
        }
        initGraphApi(renderApi, params);

        PipelineType settingPipelineType;
        try
        {
            settingPipelineType = std::any_cast<PipelineType>(params[2]);
        }
        catch (const std::bad_any_cast &e)
        {
            std::cerr << "Initialization failed: " << e.what() << std::endl;
            return false;
        }
        initRenderPipeline(settingPipelineType, params);
        bool flag = renderPipeline != nullptr;
        setInit(flag);
        return flag;
    }

    void RenderManager::initGraphApi(RenderApiType renderApi, const std::vector<std::any> &params)
    {
        renderApiType = renderApi;
        switch (renderApiType)
        {
            case RenderApiType::OpenGL:
            {
                GLFWwindow *window;
                try
                {
                    window = std::any_cast<GLFWwindow *>(params[1]);
                }
                catch (const std::bad_any_cast &e)
                {
                    std::cerr << "Initialization failed: " << e.what() << std::endl;
                }
                OpenGLInitializer oglInitializer;
                if (!oglInitializer.init({window}))
                {
                    std::cerr << "OpenGL Initialization failed." << std::endl;
                }
                graphApi = std::make_shared<OpenGLApi>();
                break;
            }
            case RenderApiType::Vulkan:
                break;
            case RenderApiType::DirectX:
                break;
        }
    }

    void RenderManager::initRenderPipeline(PipelineType settingPipelineType, const std::vector<std::any> &params)
    {
        pipelineType = settingPipelineType;
        switch (pipelineType)
        {
            case PipelineType::FORWARD:
                break;
            case PipelineType::DEFERRED:
                break;
            case PipelineType::PROGRAMMABLE:
                renderPipeline = std::make_shared<ProgrammablePipeline>(graphApi);
                break;
        }
    }

    RenderApiType RenderManager::getCurrentRenderApiType() const
    {
        return renderApiType;
    }

    std::string RenderManager::getGraphApiClassName() const
    {
        switch (renderApiType)
        {
            case RenderApiType::OpenGL:
                return "OpenGLApi";
            case RenderApiType::Vulkan:
                return "VulkanApi";
            case RenderApiType::DirectX:
                return "DirectXApi";
            default:
                throw std::runtime_error("Unknown RenderApiType");
        }
    }

    std::shared_ptr<OpenGLApi> RenderManager::getOpenGLApi()
    {
        if (renderApiType != RenderApiType::OpenGL)return nullptr;
        return getGraphApiAs<OpenGLApi>();
    }

    void RenderManager::render()
    {
        renderPipeline->render();
    }

    void RenderManager::submitEntity(std::shared_ptr<base::IRenderable> &object, RenderType renderType)
    {
        renderPipeline->submitEntity(object, renderType);
    }

    void RenderManager::submitDrawCall(std::shared_ptr<DrawCall> &drawCall, RenderType renderType)
    {
        renderPipeline->submitDrawCall(drawCall, renderType);
    }

    PipelineType RenderManager::getRenderPipelineType() const
    {
        return pipelineType;
    }

    void RenderManager::updateCameraInfo(const std::shared_ptr<assets::scene::CameraEntity> &mainCameraEntity)
    {
        if (!mainCameraEntity)
        {
            std::cerr << "Error: mainCameraEntity is null." << std::endl;
            return;
        }
        render::viewMatrix = mainCameraEntity->getViewMatrix();
        render::projectionMatrix = mainCameraEntity->getProjectionMatrix();
        render::vpMatrix = projectionMatrix * viewMatrix;
    }

    template<DerivedFromBase T>
    std::shared_ptr<T> RenderManager::getGraphApiAs() const
    {
        return std::dynamic_pointer_cast<T>(graphApi);
    }
}
