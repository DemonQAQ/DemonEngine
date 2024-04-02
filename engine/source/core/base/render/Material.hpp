//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_MATERIAL_HPP
#define DEMONENGINE_MATERIAL_HPP

#include <glm/glm.hpp>
#include <utility>
#include <vector>
#include <map>
#include "Texture.hpp"
#include "core/base/interface/INameable.hpp"

namespace base
{
    class Material : implements Object, INameable
    {
    private:
        std::string name;
        glm::vec3 diffuse;                 // 漫反射颜色
        glm::vec3 specular;                // 镜面反射颜色
        glm::vec3 ambient;                 // 环境光颜色
        glm::vec3 emissive;                // 自发光颜色
        float shininess;                   // 光泽度
        float opacity;                     // 不透明度
        float roughness;                   // 粗糙度
        float metallic;                    // 金属度
        float reflectivity;                // 反射率
        std::map<TextureType, std::map<std::shared_ptr<base::UUID>, std::shared_ptr<Texture>>> textures;
    public:
        explicit Material(std::string name_ = "Unnamed Material",
                          const glm::vec3 &diffuse_ = glm::vec3(0.8f, 0.8f, 0.8f),
                          const glm::vec3 &specular_ = glm::vec3(1.0f, 1.0f, 1.0f),
                          const glm::vec3 &ambient_ = glm::vec3(0.2f, 0.2f, 0.2f),
                          const glm::vec3 &emissive_ = glm::vec3(0.0f, 0.0f, 0.0f),
                          float shininess_ = 32.0f,
                          float opacity_ = 1.0f,
                          float roughness_ = 0.5f,
                          float metallic_ = 0.0f,
                          float reflectivity_ = 0.5f,
                          const std::map<TextureType, std::map<std::shared_ptr<base::UUID>, std::shared_ptr<Texture>>>& textures_ = {})
                : name(std::move(name_)), diffuse(diffuse_), specular(specular_), ambient(ambient_),
                  emissive(emissive_), shininess(shininess_), opacity(opacity_), roughness(roughness_),
                  metallic(metallic_), reflectivity(reflectivity_), textures(textures_)
        {}

        void addTexture(const TextureType type, const std::shared_ptr<base::UUID> &uuid, std::shared_ptr<Texture> &texture)
        {
            textures[type][uuid] = texture;
        }

        std::shared_ptr<Texture> getTexture(const TextureType type, const std::shared_ptr<base::UUID> &uuid)
        {
            auto typeIt = textures.find(type);
            if (typeIt != textures.end())
            {
                auto uuidIt = typeIt->second.find(uuid);
                if (uuidIt != typeIt->second.end())return uuidIt->second;
            }
            return nullptr;
        }

        [[nodiscard]] const std::map<TextureType, std::map<std::shared_ptr<base::UUID>, std::shared_ptr<Texture>>> &getTextures() const
        {
            return textures;
        }

        void setName(const std::string &name_) override
        {
            name = name_;
        }

        [[nodiscard]] std::string getName() const override
        {
            return name;
        }

        [[nodiscard]] glm::vec3 getDiffuse() const
        {
            return diffuse;
        }

        [[nodiscard]] glm::vec3 getSpecular() const
        {
            return specular;
        }

        [[nodiscard]] float getShininess() const
        {
            return shininess;
        }

        void setDiffuse(const glm::vec3 &diffuse_)
        {
            Material::diffuse = diffuse;
        }

        void setSpecular(const glm::vec3 &specular_)
        {
            Material::specular = specular;
        }

        [[nodiscard]] const glm::vec3 &getAmbient() const
        {
            return ambient;
        }

        void setAmbient(const glm::vec3 &ambient_)
        {
            Material::ambient = ambient;
        }

        [[nodiscard]] const glm::vec3 &getEmissive() const
        {
            return emissive;
        }

        void setEmissive(const glm::vec3 &emissive_)
        {
            Material::emissive = emissive;
        }

        void setShininess(float shininess_)
        {
            Material::shininess = shininess;
        }

        [[nodiscard]] float getOpacity() const
        {
            return opacity;
        }

        void setOpacity(float opacity_)
        {
            Material::opacity = opacity;
        }

        [[nodiscard]] float getRoughness() const
        {
            return roughness;
        }

        void setRoughness(float roughness_)
        {
            Material::roughness = roughness;
        }

        [[nodiscard]] float getMetallic() const
        {
            return metallic;
        }

        void setMetallic(float metallic_)
        {
            Material::metallic = metallic;
        }

        [[nodiscard]] float getReflectivity() const
        {
            return reflectivity;
        }

        void setReflectivity(float reflectivity_)
        {
            Material::reflectivity = reflectivity;
        }
    };
}

#endif //DEMONENGINE_MATERIAL_HPP