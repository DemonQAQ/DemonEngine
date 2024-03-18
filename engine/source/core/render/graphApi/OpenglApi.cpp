//
// Created by Demon on 2024/3/19.
//
#include "OpenglApi.hpp"

using namespace render;

void OpenglApi::bindContext(GLFWwindow *window)
{
    this->currentWindow = window;
}

void OpenglApi::unbindContext()
{
    this->currentWindow = nullptr;
}

void OpenglApi::drawLine(float x1, float y1, float x2, float y2)
{

}

void OpenglApi::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3)
{

}

void OpenglApi::drawRectangle(float x, float y, float width, float height)
{

}

void OpenglApi::drawCircle(float centerX, float centerY, float radius)
{

}

void OpenglApi::drawMesh(std::shared_ptr<base::Mesh> mesh)
{

}

void OpenglApi::drawModel(std::shared_ptr<base::Model> model)
{

}

void OpenglApi::executeDrawCall(const DrawCall &drawCall)
{

}

void OpenglApi::useShader(std::shared_ptr<base::Shader> shader)
{

}

void OpenglApi::releaseShader(std::shared_ptr<base::Shader> shader)
{

}

void OpenglApi::bindTexture(std::shared_ptr<base::Texture> texture)
{

}

void OpenglApi::unbindTexture(std::shared_ptr<base::Texture> texture)
{

}

void OpenglApi::setBlendMode(BlendMode mode)
{

}

void OpenglApi::setViewport(int x, int y, int width, int height)
{

}

void OpenglApi::clear(float r, float g, float b, float a)
{

}

void OpenglApi::enableDepthTest(bool enable)
{

}

void OpenglApi::setDepthFunction(DepthFunction func)
{

}

void OpenglApi::enableCulling(bool enable)
{

}

void OpenglApi::setCullFace(CullFace face)
{

}

