//
// Created by Demon on 2024/3/24.
//

#include "OpenglDrawCall.hpp"

void render::OpenglDrawCall::preExecute()
{
    // 首先合并所有顶点和索引数据
    std::vector<base::Vertex> allVertices;
    std::vector<unsigned int> allIndices;
    unsigned int indexOffset = 0;

    for (const auto &renderDataItem: data)
    {
        allVertices.insert(allVertices.end(),
                           renderDataItem.vertices.begin(),
                           renderDataItem.vertices.end());
        // 需要添加当前索引的偏移量
        for (auto index : renderDataItem.indices)
        {
            allIndices.push_back(index + indexOffset);
        }
        // 更新偏移量
        indexOffset += renderDataItem.vertices.size();
    }

    // 创建VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 创建VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(base::Vertex),
                 allVertices.data(), GL_STATIC_DRAW);

    // 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(base::Vertex),
                          (void *) offsetof(base::Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(base::Vertex),
                          (void *) offsetof(base::Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(base::Vertex),
                          (void *) offsetof(base::Vertex, texCoords));
    glEnableVertexAttribArray(2);

    // 创建EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, allIndices.size() * sizeof(unsigned int),
                 allIndices.data(), GL_STATIC_DRAW);

    vertexCount = allVertices.size();
    indexCount = allIndices.size();

    // 解绑VBO和VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void render::OpenglDrawCall::afterExecute()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
