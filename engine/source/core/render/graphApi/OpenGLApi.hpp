//
// Created by Demon on 2023/12/12.
//

#ifndef DEMONENGINE_OPENGLAPI_HPP
#define DEMONENGINE_OPENGLAPI_HPP

#include "core/base/interface/Interface.hpp"
#include "GraphApi.hpp"

namespace render
{
    //todo 池化管理VAO/VBO
    class OpenGLApi : implements GraphApi
    {
    private:
        GLFWwindow *currentWindow;
        std::shared_ptr<base::Shader> normalShader;
        std::shared_ptr<base::Shader> usingShader;
    public:
        void init(const std::vector<std::any> &params) override;

        void bindContext(GLFWwindow *window) override;

        void unbindContext() override;

        void drawLine(float x1, float y1, float x2, float y2, const glm::vec4 &color) override;

        void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const glm::vec4 &color) override;

        void drawRectangle(float x, float y, float width, float height, const glm::vec4 &color) override;

        void drawCircle(float centerX, float centerY, float radius, const glm::vec4 &color) override;

        void drawImage(const std::shared_ptr<base::Texture> &texture, float x, float y, float width, float height,
                       const glm::vec4 &color = glm::vec4(1.0f)) override;

        void drawMesh(std::shared_ptr<base::Mesh> mesh) override;

        void drawModel(std::shared_ptr<base::Model> model) override;

        void executeDrawCall(const DrawCall &drawCall) override;

        void useShader(std::shared_ptr<base::Shader> shader) override;

        std::shared_ptr<base::Shader> getUsingShader() override;

        void releaseShader() override;

        void bindTexture(std::shared_ptr<base::Texture> texture) override;

        void bindTextureWithParam(const std::shared_ptr<base::Texture> &texture, GLenum textureUnit = GL_TEXTURE0,
                                    const std::map<GLenum, GLint>& parameters = {});

        void unbindTexture(const std::vector<std::any>& params) override;

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
