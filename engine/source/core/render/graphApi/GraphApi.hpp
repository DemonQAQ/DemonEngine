﻿//
// Created by Demon on 2023/12/12.
//

#ifndef DEMONENGINE_GRAPHAPI_HPP
#define DEMONENGINE_GRAPHAPI_HPP

#define M_PI 3.14159265358979323846

#include "glm/vec4.hpp"
#include "core/base/render/Material.hpp"
#include "core/base/render/Shader.hpp"
#include "core/base/render/Texture.hpp"
#include "core/base/render/Mesh.hpp"
#include <memory>
#include <any>
#include "core/base/interface/Interface.hpp"
#include "GLFW/glfw3.h"

namespace render
{
    class DrawCall;

    enum class BlendMode
    {
        BLEND,
        ALPHA,
        ADDITIVE
    };

    enum class DepthFunction
    {
        LESS,
        GREATER,
        LEQUAL,
        GEQUAL,
        EQUAL,
        NOTEQUAL,
        ALWAYS
    };

    enum class CullFace
    {
        FRONT,
        BACK,
        FRONT_AND_BACK
    };

    interface GraphApi
    {
    public:
        virtual void init(const std::vector<std::any>& params) = 0;

        virtual void bindContext(GLFWwindow* window) = 0;
        virtual void unbindContext() = 0;

        // 基础绘制操作
        virtual void drawLine(float x1, float y1, float x2, float y2, const glm::vec4& color) = 0;
        virtual void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const glm::vec4& color) = 0;
        virtual void drawRectangle(float x, float y, float width, float height, const glm::vec4& color) = 0;
        virtual void drawCircle(float centerX, float centerY, float radius, const glm::vec4& color) = 0;

        virtual void drawImage(const std::shared_ptr<base::Texture>& texture, float x, float y, float width, float height, const glm::vec4& color = glm::vec4(1.0f)) = 0;
        virtual void drawMesh(std::shared_ptr<base::Mesh> mesh) = 0;
        virtual void drawModel(std::shared_ptr<base::Model> model) = 0;
        virtual void executeDrawCall(std::shared_ptr<DrawCall> drawCall) = 0;

        virtual void useShader(std::shared_ptr<base::Shader> shader) = 0;
        virtual std::shared_ptr<base::Shader> getUsingShader()=0;
        virtual void releaseShader() = 0;

        virtual void bindTexture(std::shared_ptr<base::Texture> texture) = 0;
        virtual void unbindTexture(const std::vector<std::any>& params) = 0;

        // 渲染状态管理
        virtual void setBlendMode(BlendMode mode) = 0;
        virtual void setViewport(int x, int y, int width, int height) = 0;
        virtual void setClearColor(float r, float g, float b, float a) = 0;
        virtual void clear() = 0;

        virtual void enableBlendMode(bool enable) = 0;
        virtual void enableDepthTest(bool enable) = 0; // 启用\禁用深度测试
        virtual void setDepthFunction(DepthFunction func) = 0; // 设置深度测试函数
        virtual void enableCulling(bool enable) = 0; // 启用/禁用面剔除
        virtual void setCullFace(CullFace face) = 0; // 设置剔除面
    };
}

#endif //DEMONENGINE_GRAPHAPI_HPP
