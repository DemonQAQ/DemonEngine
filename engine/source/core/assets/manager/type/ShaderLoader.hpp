//
// Created by Demon on 2024/4/13.
//

#ifndef DEMONENGINE_SHADERLOADER_HPP
#define DEMONENGINE_SHADERLOADER_HPP

#include <core/base/render/Shader.hpp>
#include <memory>
#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IFileLoader.hpp"

namespace assets
{
    class ShaderLoader : implements IFileLoader
    {
    public:
        static std::shared_ptr<base::Shader> loadShader(const std::string& name, const std::string &metaPath, const std::string &vertexPath, const std::string &fragmentPath);
    };
}

#endif //DEMONENGINE_SHADERLOADER_HPP
