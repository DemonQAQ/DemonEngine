//
// Created by Demon on 2023/6/10.
//

#ifndef DEMON_ENGINE_SHADER_HPP
#define DEMON_ENGINE_SHADER_HPP

#include "glad/glad.h" // 包含glad来获取所有的必须OpenGL头文件
#include "glm/glm.hpp"
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace base
{
    class Shader : implements Object
    {
    public:
        // 程序ID
        unsigned int ID;

        // 构造器读取并构建着色器
        Shader(const std::string& vertexPath, const std::string& fragmentPath);

        // 使用/激活程序
        void use();

        // uniform工具函数
        void setBool(const std::string &name, bool value) const;

        void setInt(const std::string &name, int value) const;

        void setFloat(const std::string &name, float value) const;

        void setVec2(const std::string &name, const glm::vec2 &value) const;

        void setVec2(const std::string &name, float x, float y) const;

        void setVec3(const std::string &name, const glm::vec3 &value) const;

        void setVec3(const std::string &name, float x, float y, float z) const;

        void setVec4(const std::string &name, const glm::vec4 &value) const;

        void setVec4(const std::string &name, float x, float y, float z, float w);

        void setMat2(const std::string &name, const glm::mat2 &mat) const;

        void setMat3(const std::string &name, const glm::mat3 &mat) const;

        void setMat4(const std::string &name, const glm::mat4 &mat) const;

    private:
        void checkCompileErrors(unsigned int shader, std::string type);
    };

}
#endif //DEMON_ENGINE_SHADER_HPP
