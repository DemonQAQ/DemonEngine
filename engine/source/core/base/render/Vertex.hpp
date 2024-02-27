//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_VERTEX_HPP
#define DEMONENGINE_VERTEX_HPP

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

namespace base
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec3 tangent;
        glm::vec3 bitangent;

        Vertex() : position(glm::vec3(0.0f)), normal(glm::vec3(0.0f)), texCoords(glm::vec2(0.0f)),
                   tangent(glm::vec3(0.0f)), bitangent(glm::vec3(0.0f))
        {}

        Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex, glm::vec3 tan = glm::vec3(0.0f),
               glm::vec3 bitan = glm::vec3(0.0f))
                : position(pos), normal(norm), texCoords(tex), tangent(tan), bitangent(bitan)
        {}
    };
}

#endif //DEMONENGINE_VERTEX_HPP
