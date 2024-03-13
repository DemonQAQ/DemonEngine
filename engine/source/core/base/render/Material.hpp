//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_MATERIAL_HPP
#define DEMONENGINE_MATERIAL_HPP

#include <glm/glm.hpp>
#include <vector>
#include "Texture.hpp"

namespace base
{
    class Material : implements Object
    {
    public:
        glm::vec3 diffuse;                 // 漫反射颜色
        glm::vec3 specular;                // 镜面反射颜色
        float shininess;                   // 光泽度
        std::vector<Texture> textures;     // 材质使用的贴图

        Material(const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
                : diffuse(diffuse), specular(specular), shininess(shininess) {}

        void addTexture(const Texture& texture)
        {
            textures.push_back(texture);
        }
    };
}

#endif //DEMONENGINE_MATERIAL_HPP