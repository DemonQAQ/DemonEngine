//
// Created by Demon on 2023/12/12.
//

#ifndef DEMONENGINE_OPENGLAPI_HPP
#define DEMONENGINE_OPENGLAPI_HPP

#include "core/base/interface/Interface.hpp"
#include "GraphApi.hpp"

namespace render
{
    class OpenglApi : implements GraphApi
    {
    private:
        GLFWwindow* currentWindow;
    public:
        void bindContext(GLFWwindow* window) override;
        void unbindContext() override;

        void drawLine(float x1, float y1, float x2, float y2) override;
        void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) override;
        void drawRectangle(float x, float y, float width, float height) override;
        void drawCircle(float centerX, float centerY, float radius) override;

        void drawMesh(std::shared_ptr<base::Mesh> mesh) override;
        void drawModel(std::shared_ptr<base::Model> model) override;
        void executeDrawCall(const DrawCall& drawCall) override;

        void useShader(std::shared_ptr<base::Shader> shader) override;
        void releaseShader(std::shared_ptr<base::Shader> shader) override;

        void bindTexture(std::shared_ptr<base::Texture> texture) override;
        void unbindTexture(std::shared_ptr<base::Texture> texture) override;

        void setBlendMode(BlendMode mode) override;
        void setViewport(int x, int y, int width, int height) override;
        void clear(float r, float g, float b, float a) override;

        void enableDepthTest(bool enable) override;
        void setDepthFunction(DepthFunction func) override;
        void enableCulling(bool enable) override;
        void setCullFace(CullFace face) override;
    };
}

#endif //DEMONENGINE_OPENGLAPI_HPP
