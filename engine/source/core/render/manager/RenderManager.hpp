//
// Created by Demon on 2024/3/24.
//

#ifndef DEMONENGINE_RENDERMANAGER_HPP
#define DEMONENGINE_RENDERMANAGER_HPP

#include <core/assets/scene/CameraEntity.hpp>
#include "glm/detail/type_mat4x4.hpp"
#include "core/base/interface/Interface.hpp"
#include "core/render/manager/Initializer.hpp"
#include "core/render/graphApi/GraphApi.hpp"
#include "core/render/manager/OpenGLInitializer.hpp"
#include "core/render/graphApi/OpenGLApi.hpp"
#include "core/render/pipeline/RenderPipeline.hpp"

namespace render
{
    template<typename T>
    concept DerivedFromBase = std::is_base_of_v<GraphApi, T>;

    extern glm::mat4 vpMatrix;
    extern glm::mat4 viewMatrix;
    extern glm::mat4 projectionMatrix;

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
        PipelineType pipelineType = PipelineType::PROGRAMMABLE;
        std::shared_ptr<RenderPipeline> renderPipeline = nullptr;
    public:
        RenderManager() = default;

        bool init(const std::vector<std::any> &params) override;

        [[nodiscard]] RenderApiType getCurrentRenderApiType() const;

        [[nodiscard]] std::string getGraphApiClassName() const;

        [[nodiscard]] PipelineType getRenderPipelineType() const;

        template<DerivedFromBase T>
        [[nodiscard]] std::shared_ptr<T> getGraphApiAs() const;

        std::shared_ptr<OpenGLApi> getOpenGLApi();

        void setViewport(int x, int y, int width, int height);

        void updateCameraInfo(const std::shared_ptr<assets::scene::CameraEntity> &mainCameraEntity);

        void render();

        void submitEntity(std::shared_ptr<base::IRenderable> &object, RenderType renderType);

        void submitDrawCall(std::shared_ptr<DrawCall> &drawCall, RenderType renderType);
    private:
        void initGraphApi(RenderApiType renderApi, const std::vector<std::any> &params);

        void initRenderPipeline(PipelineType settingPipelineType, const std::vector<std::any> &params);
    };
}

#endif //DEMONENGINE_RENDERMANAGER_HPP
