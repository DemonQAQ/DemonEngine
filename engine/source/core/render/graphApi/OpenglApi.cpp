//
// Created by Demon on 2024/3/19.
//
#include "OpenglApi.hpp"

using namespace render;

void OpenglApi::init(const std::vector<std::any> &params)
{
    if (params.empty()) return;

    if (params[0].type() == typeid(const std::shared_ptr<base::Shader>))
    {
        this->normalShader = std::any_cast<std::shared_ptr<base::Shader>>(params[0]);
        useShader(normalShader);
    }
}

void OpenglApi::bindContext(GLFWwindow *window)
{
    this->currentWindow = window;
}

void OpenglApi::unbindContext()
{
    this->currentWindow = nullptr;
}

void OpenglApi::drawLine(float x1, float y1, float x2, float y2, const glm::vec4 &color)
{
    float vertices[] = {
            x1, y1, 0.0f, // Point 1
            x2, y2, 0.0f  // Point 2
    };

    auto shader = getUsingShader();
    if (!shader)
    {
        std::cerr << "No shader is currently in use. Cannot draw line." << std::endl;
        return;
    }

    shader->use();
    //todo 需要标准化shader的颜色变量名称
    shader->setVec4("uColor", color);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Assuming the vertex positions are linked to attribute location 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // Draw the line
    glDrawArrays(GL_LINES, 0, 2);

    // Cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void OpenglApi::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const glm::vec4 &color)
{
    float vertices[] = {
            x1, y1, 0.0f, // Point 1
            x2, y2, 0.0f, // Point 2
            x3, y3, 0.0f  // Point 3
    };

    auto shader = getUsingShader();
    if (!shader)
    {
        std::cerr << "No shader is currently in use. Cannot draw triangle." << std::endl;
        return;
    }

    shader->use();
    shader->setVec4("uColor", color);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void OpenglApi::drawRectangle(float x, float y, float width, float height, const glm::vec4 &color)
{
    float vertices[] = {
            x, y, 0.0f, // Left bottom
            x + width, y, 0.0f, // Right bottom
            x, y + height, 0.0f, // Left top
            x + width, y + height, 0.0f // Right top
    };

    unsigned int indices[] = { // Note that we start from 0!
            0, 1, 2, // First Triangle
            1, 3, 2  // Second Triangle
    };

    auto shader = getUsingShader();
    if (!shader)
    {
        std::cerr << "No shader is currently in use. Cannot draw rectangle." << std::endl;
        return;
    }

    shader->use();
    shader->setVec4("uColor", color);

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind EBO AFTER unbinding VAO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void OpenglApi::drawCircle(float centerX, float centerY, float radius, const glm::vec4 &color)
{
    const int segments = 36;
    float vertices[(segments + 2) * 3]; // Extra for the center and to close the circle

    // Center vertex
    vertices[0] = centerX;
    vertices[1] = centerY;
    vertices[2] = 0.0f;

    float angleIncrement = 2.0f * M_PI / segments;
    for (int i = 1; i <= segments + 1; ++i)
    {
        float angle = angleIncrement * i;
        vertices[i * 3 + 0] = centerX + cos(angle) * radius; // X
        vertices[i * 3 + 1] = centerY + sin(angle) * radius; // Y
        vertices[i * 3 + 2] = 0.0f; // Z
    }

    auto shader = getUsingShader();
    if (!shader)
    {
        std::cerr << "No shader is currently in use. Cannot draw circle." << std::endl;
        return;
    }

    shader->use();
    shader->setVec4("uColor", color);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void OpenglApi::drawImage(const std::shared_ptr<base::Texture> &texture, float x, float y, float width, float height,
                          const glm::vec4 &color)
{
    if (!texture)
    {
        std::cerr << "Texture is null, cannot draw image." << std::endl;
        return;
    }

    auto shader = getUsingShader();
    shader->use();
    shader->setInt("uTexture", 0);
    shader->setVec4("uColor", color);

    float vertices[] = {
            // 位置          // 纹理坐标
            x + width, y + height, 0.0f, 1.0f,
            x + width, y, 0.0f, 0.0f,
            x, y, 1.0f, 0.0f,
            x, y + height, 1.0f, 1.0f
    };

    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 位置属性
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // 纹理坐标属性
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    // 绘制图片
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void OpenglApi::drawMesh(std::shared_ptr<base::Mesh> mesh)
{
    //todo 实现getRenderData
    if (!mesh) return;

    auto &vertices = mesh->getVertices();
    auto &indices = mesh->getIndices();
    auto material = mesh->getMaterial();

    if (!usingShader)useShader(normalShader);

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(base::Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(base::Vertex), (void *) 0);
    glEnableVertexAttribArray(0);
    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(base::Vertex), (void *) offsetof(base::Vertex, normal));
    glEnableVertexAttribArray(1);
    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(base::Vertex), (void *) offsetof(base::Vertex, texCoords));
    glEnableVertexAttribArray(2);
    // Tangent attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(base::Vertex), (void *) offsetof(base::Vertex, tangent));
    glEnableVertexAttribArray(3);
    // Bitangent attribute
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(base::Vertex), (void *) offsetof(base::Vertex, bitangent));
    glEnableVertexAttribArray(4);

    // Bind appropriate textures
    for (const auto &texPair: material->getTextures())
    {
        for (const auto &uuidTexPair: texPair.second)
        {
            auto texture = uuidTexPair.second;
            bindTexture(texture);
        }
    }

    // Draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    releaseShader();
}

void OpenglApi::drawModel(std::shared_ptr<base::Model> model)
{

}

void OpenglApi::executeDrawCall(const DrawCall &drawCall)
{

}

void OpenglApi::useShader(std::shared_ptr<base::Shader> shader)
{
    if (shader)
    {
        glUseProgram(shader->ID);
        usingShader = shader;
    } else
    {
        glUseProgram(normalShader->ID);
        usingShader = normalShader;
        std::cerr << "Shader is nullptr, cannot use it, using normal shader." << std::endl;
    }
}

void OpenglApi::releaseShader()
{
    glUseProgram(normalShader->ID);
    usingShader = normalShader;
}

std::shared_ptr<base::Shader> OpenglApi::getUsingShader()
{
    return usingShader;
}

void OpenglApi::bindTexture(std::shared_ptr<base::Texture> texture)
{
    bindTextureWithParam(texture, GL_TEXTURE0, {});
}

void OpenglApi::bindTextureWithParam(const std::shared_ptr<base::Texture> &texture, GLenum textureUnit,
                                     const std::map<GLenum, GLint> &parameters)
{
    if (!texture) return;
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    for (const auto &param: parameters)
    {
        glTexParameteri(GL_TEXTURE_2D, param.first, param.second);
    }
}

void OpenglApi::unbindTexture(const std::vector<std::any> &params)
{
    if (params.empty()) return;

    if (params[0].type() == typeid(const GLenum))
    {
        glActiveTexture(std::any_cast<GLenum>(params[0]));
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void OpenglApi::setBlendMode(BlendMode mode)
{
    glEnable(GL_BLEND);
    switch (mode)
    {
        case BlendMode::NONE:
            glDisable(GL_BLEND);
            break;
        case BlendMode::ALPHA:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case BlendMode::ADDITIVE:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        default:
            break;
    }
}

void OpenglApi::setViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void OpenglApi::clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenglApi::enableDepthTest(bool enable)
{
    if (enable) glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
}

void OpenglApi::setDepthFunction(DepthFunction func)
{
    GLenum glFunc;
    switch (func)
    {
        case DepthFunction::LESS:
            glFunc = GL_LESS;
            break;
        case DepthFunction::GREATER:
            glFunc = GL_GREATER;
            break;
        case DepthFunction::LEQUAL:
            glFunc = GL_LEQUAL;
            break;
        case DepthFunction::GEQUAL:
            glFunc = GL_GEQUAL;
            break;
        case DepthFunction::EQUAL:
            glFunc = GL_EQUAL;
            break;
        case DepthFunction::NOTEQUAL:
            glFunc = GL_NOTEQUAL;
            break;
        case DepthFunction::ALWAYS:
            glFunc = GL_ALWAYS;
            break;
        default:
            glFunc = GL_LESS;
    }
    glDepthFunc(glFunc);
}

void OpenglApi::enableCulling(bool enable)
{
    if (enable)glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
}

void OpenglApi::setCullFace(CullFace face)
{
    GLenum glFace;
    switch (face)
    {
        case CullFace::FRONT:
            glFace = GL_FRONT;
            break;
        case CullFace::BACK:
            glFace = GL_BACK;
            break;
        case CullFace::FRONT_AND_BACK:
            glFace = GL_FRONT_AND_BACK;
            break;
        default:
            glFace = GL_BACK;
    }
    glCullFace(glFace);
}



