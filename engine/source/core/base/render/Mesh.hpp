//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_MESH_HPP
#define DEMONENGINE_MESH_HPP

#include <vector>
#include <string>
#include "Texture.hpp"
#include "Vertex.hpp"

namespace base
{
    class Mesh
    {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
             const std::vector<Texture> &textures)
                : vertices(vertices), indices(indices), textures(textures)
        {}
    };
}

#endif //DEMONENGINE_MESH_HPP
