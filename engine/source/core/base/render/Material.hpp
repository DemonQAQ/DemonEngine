//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_MATERIAL_HPP
#define DEMONENGINE_MATERIAL_HPP

#include <glm/glm.hpp>
#include <utility>
#include <vector>
#include <map>
#include <core/base/interface/IMetaAccessor.hpp>
#include <core/base/common/manager/UUIDManager.hpp>
#include "Texture.hpp"
#include "core/base/interface/INameable.hpp"

namespace base
{
    class MaterialBlockOperator : implements BlockOperator
    {
        void writeToBlock(std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml)
        {
            auto materialsUUID = std::any_cast<std::vector<std::shared_ptr<UUID>>>(metadata->getValue("textures"));
            std::vector<std::string> materials;
            materials.reserve(materialsUUID.size());
            yml->set("MaterialBlockOperator.textures", materials);
        }

        void readFromBlock(std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml)
        {
            auto materialsVec = yml->getStringList("MaterialBlockOperator.textures");
            std::vector<std::shared_ptr<UUID>> materialsUUID;
            materialsUUID.reserve(materialsVec.size());
            for (const auto &material: materialsVec) materialsUUID.push_back(UUIDManager::getUUID(material));
            metadata->setValue("textures", materialsUUID);
        }

        void initBlock(std::shared_ptr<Metadata> &metadata, const std::vector<std::any> &params)
        {
            if (!params.empty() && params[0].type() == typeid(std::vector<std::string>))
            {
                std::vector<std::string> materials = std::any_cast<std::vector<std::string>>(params[0]);
                std::vector<std::shared_ptr<UUID>> materialsUUID = {};
                materialsUUID.reserve(materials.size());
                for (const auto &material: materials)materialsUUID.push_back(UUIDManager::getUUID(material));
                metadata->setValue("textures", materialsUUID);
            }
            else metadata->setValue("textures", std::vector<std::string>{});
        }
    };

    class Material : implements Object, implements INameable, implements IMetaAccessor
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
        explicit Material(const std::shared_ptr<base::UUID> &existingUuid,
                          bool init,
                          std::shared_ptr<io::YamlConfiguration> &yml,
                          std::string name_ = "Unnamed Material",
                          const glm::vec3 &diffuse_ = glm::vec3(0.8f, 0.8f, 0.8f),
                          const glm::vec3 &specular_ = glm::vec3(1.0f, 1.0f, 1.0f),
                          const glm::vec3 &ambient_ = glm::vec3(0.2f, 0.2f, 0.2f),
                          const glm::vec3 &emissive_ = glm::vec3(0.0f, 0.0f, 0.0f),
                          float shininess_ = 32.0f,
                          float opacity_ = 1.0f,
                          float roughness_ = 0.5f,
                          float metallic_ = 0.0f,
                          float reflectivity_ = 0.5f,
                          const std::map<TextureType, std::map<std::shared_ptr<base::UUID>, std::shared_ptr<Texture>>> &textures_ = {})
                : Object(existingUuid),
                  IMetaAccessor(yml, !init, init ? nullptr : existingUuid),
                  name(std::move(name_)), diffuse(diffuse_), specular(specular_), ambient(ambient_),
                  emissive(emissive_), shininess(shininess_), opacity(opacity_), roughness(roughness_),
                  metallic(metallic_), reflectivity(reflectivity_), textures(textures_)
        {}

        void init()
        {
            addOperator(std::make_shared<MaterialBlockOperator>());
        }

        void setTextures(
                const std::map<TextureType, std::map<std::shared_ptr<base::UUID>, std::shared_ptr<Texture>>> &newTextures)
        {
            textures = newTextures;
        }

        void addTexture(std::shared_ptr<Texture> &texture)
        {
            textures[texture->type][texture->getUUID()] = texture;
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

        [[nodiscard]] const std::map<TextureType, std::map<std::shared_ptr<base::UUID>, std::shared_ptr<Texture>>> &
        getTextures() const
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