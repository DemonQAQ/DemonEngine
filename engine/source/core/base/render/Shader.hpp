//
// Created by Demon on 2023/6/10.
//

#ifndef DEMON_ENGINE_SHADER_HPP
#define DEMON_ENGINE_SHADER_HPP

#include "core/base/lib/pch.hpp"
#include "glm/glm.hpp"
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <core/base/interface/IMetaAccessor.hpp>
#include <core/base/interface/INameable.hpp>

namespace base
{
    class ShaderBlockOperator : implements BlockOperator
    {
        void writeToBlock(const std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml) override
        {
            try
            {
                auto vertexPath = std::any_cast<std::string>(metadata->getValueOrDefault("vertexPath", std::string("none")));
                auto fragmentPath = std::any_cast<std::string>(metadata->getValueOrDefault("fragmentPath", std::string("none")));
                auto name = std::any_cast<std::string>(metadata->getValueOrDefault("name", std::string("normal shader")));

                yml->set("ShaderBlockOperator.vertexPath", vertexPath);
                yml->set("ShaderBlockOperator.fragmentPath", fragmentPath);
                yml->set("ShaderBlockOperator.name", name);
            } catch (const std::bad_any_cast &)
            {
                // Handle or log error
            }
        }

        void readFromBlock(const std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml) override
        {
            const std::string defaultVertexPath = "none";
            const std::string defaultFragmentPath = "none";
            const std::string defaultName = "normal shader";

            std::string vertexPath = yml->getString("ShaderBlockOperator.vertexPath");
            if (vertexPath.empty())
            {
                vertexPath = defaultVertexPath;
            }

            std::string fragmentPath = yml->getString("ShaderBlockOperator.fragmentPath");
            if (fragmentPath.empty())
            {
                fragmentPath = defaultFragmentPath;
            }

            std::string name = yml->getString("ShaderBlockOperator.name");
            if (name.empty())
            {
                name = defaultName;
            }

            metadata->setValue("vertexPath", vertexPath);
            metadata->setValue("fragmentPath", fragmentPath);
            metadata->setValue("name", name);
        }

        void initBlock(std::shared_ptr<Metadata> &metadata, const std::vector<std::any> &params) override
        {
            if (params.size() > 0 && params[0].type() == typeid(std::string))
            {
                auto vertexPath = std::any_cast<std::string>(params[0]);
                metadata->setValue("vertexPath", vertexPath);
            }
            else metadata->setValue("vertexPath", "none");

            if (params.size() > 1 && params[1].type() == typeid(std::string))
            {
                auto fragmentPath = std::any_cast<std::string>(params[1]);
                metadata->setValue("fragmentPath", fragmentPath);
            }
            else metadata->setValue("fragmentPath", "none");

            if (params.size() > 2 && params[2].type() == typeid(std::string))
            {
                auto name = std::any_cast<std::string>(params[2]);
                metadata->setValue("name", name);
            }
            else metadata->setValue("name", "normal shader");
        }
    };

    class Shader : implements Object, implements IMetaAccessor, implements INameable
    {
    private:
        std::string name;
    public:
        // 程序ID
        unsigned int ID;

        // 构造器读取并构建着色器
        Shader(const std::shared_ptr<base::UUID> &existingUuid, bool init, std::string name_,
               const std::string &vertexPath, const std::string &fragmentPath,
               std::shared_ptr<io::YamlConfiguration> &yml);

        void init()
        {
            addOperator(std::make_shared<ShaderBlockOperator>());
        }

        // 使用/激活程序
        void use();

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

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
