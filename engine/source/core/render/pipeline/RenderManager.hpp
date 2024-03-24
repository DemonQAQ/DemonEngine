//
// Created by Demon on 2024/3/24.
//

#ifndef DEMONENGINE_RENDERMANAGER_HPP
#define DEMONENGINE_RENDERMANAGER_HPP

#include "glm/detail/type_mat4x4.hpp"
#include "core/base/interface/Interface.hpp"
#include "core/render/manager/Initializer.hpp"
#include "core/render/graphApi/GraphApi.hpp"
#include "core/render/manager/OpenGLInitializer.hpp"

namespace render
{
    glm::mat4 vpMatrix;
    glm::mat4 viewMatrix = glm::mat4(1.0f); // 视图矩阵
    glm::mat4 projectionMatrix = glm::mat4(1.0f); // 投影矩阵

    enum class RenderApiType
    {
        OpenGL,
        Vulkan,
        DirectX
    };

    class RenderManager : implements Initializer
    {
    private:
        RenderApiType renderApiType = RenderApiType::OpenGL;
        std::shared_ptr<GraphApi> graphApi;
    public:
        RenderManager() = default;

        bool init(const std::vector<std::any> &params) override
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
                        return false;
                    }
                    OpenGLInitializer oglInitializer;
                    if (!oglInitializer.init({window}))
                    {
                        std::cerr << "OpenGL Initialization failed." << std::endl;
                        return false;
                    }
                    graphApi = std::make_shared<OpenGLApi>();
                    break;
                }
                case RenderApiType::Vulkan:
                    break;
                case RenderApiType::DirectX:
                    break;
            }
            setInit(true);
            return true;
        }

        [[nodiscard]] RenderApiType getCurrentRenderApiType() const
        {
            return renderApiType;
        }

        [[nodiscard]] std::string getGraphApiClassName() const
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

        template<typename T>
        std::shared_ptr<T> getGraphApiAs() const
        {
            return std::dynamic_pointer_cast<T>(graphApi);
        }

        std::shared_ptr<OpenGLApi> getOpenGLApi()
        {
            if (renderApiType != RenderApiType::OpenGL)return nullptr;
            return getGraphApiAs<OpenGLApi>();
        }
    };
}

#endif //DEMONENGINE_RENDERMANAGER_HPP
