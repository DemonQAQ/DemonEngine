//
// Created by Demon on 2023/12/12.
//

#ifndef DEMONENGINE_GRAPHAPI_HPP
#define DEMONENGINE_GRAPHAPI_HPP

#include <memory>
#include "core/base/interface/Interface.hpp"
#include "core/base/render/Mesh.hpp"
#include "core/base/render/Shader.hpp"
#include "GLFW/glfw3.h"

namespace render
{
    class DrawCall;

    enum class BlendMode
    {
        NONE,
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
        virtual void bindContext(GLFWwindow* window) = 0;
        virtual void unbindContext() = 0;

        // 基础绘制操作
        virtual void drawLine(float x1, float y1, float x2, float y2) = 0;
        virtual void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) = 0;
        virtual void drawRectangle(float x, float y, float width, float height) = 0;
        virtual void drawCircle(float centerX, float centerY, float radius) = 0;

        virtual void drawMesh(std::shared_ptr<base::Mesh> mesh) = 0;
        virtual void drawModel(std::shared_ptr<base::Model> model) = 0;
        virtual void executeDrawCall(const DrawCall& drawCall) = 0;

        virtual void useShader(std::shared_ptr<base::Shader> shader) = 0;
        virtual void releaseShader(std::shared_ptr<base::Shader> shader) = 0;

        virtual void bindTexture(std::shared_ptr<base::Texture> texture) = 0;
        virtual void unbindTexture(std::shared_ptr<base::Texture> texture) = 0;

        // 渲染状态管理
        virtual void setBlendMode(BlendMode mode) = 0;
        virtual void setViewport(int x, int y, int width, int height) = 0;
        virtual void clear(float r, float g, float b, float a) = 0;

        virtual void enableDepthTest(bool enable) = 0; // 启用\禁用深度测试
        virtual void setDepthFunction(DepthFunction func) = 0; // 设置深度测试函数
        virtual void enableCulling(bool enable) = 0; // 启用/禁用面剔除
        virtual void setCullFace(CullFace face) = 0; // 设置剔除面
    };
}

#endif //DEMONENGINE_GRAPHAPI_HPP
