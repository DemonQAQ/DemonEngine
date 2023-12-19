//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_TEXTURE_HPP
#define DEMONENGINE_TEXTURE_HPP

#include <string>
#include <utility>

namespace base
{
    struct Texture
    {
        unsigned int id;       // 贴图的 OpenGL ID
        std::string type;      // 贴图类型（如 "diffuse" 或 "specular"）
        std::string path;      // 贴图文件的路径

        Texture(unsigned int id, std::string type, std::string path)
                : id(id), type(std::move(type)), path(std::move(path))
        {}
    };
}

#endif //DEMONENGINE_TEXTURE_HPP