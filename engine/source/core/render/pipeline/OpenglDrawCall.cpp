//
// Created by Demon on 2024/3/24.
//

#include "OpenglDrawCall.hpp"

void render::OpenglDrawCall::preExecute()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    for (const auto &renderDataItem: data)
    {
        if (!renderDataItem.vertices.empty())
        {
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, renderDataItem.vertices.size() * sizeof(base::Vertex),
                         renderDataItem.vertices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(base::Vertex),
                                  (void *) offsetof(base::Vertex, position));
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(base::Vertex),
                                  (void *) offsetof(base::Vertex, normal));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(base::Vertex),
                                  (void *) offsetof(base::Vertex, texCoords));
            glEnableVertexAttribArray(2);
        }

        if (!renderDataItem.indices.empty())
        {
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderDataItem.indices.size() * sizeof(unsigned int),
                         renderDataItem.indices.data(), GL_STATIC_DRAW);
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void render::OpenglDrawCall::afterExecute()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (EBO != 0)glDeleteBuffers(1, &EBO);
}
