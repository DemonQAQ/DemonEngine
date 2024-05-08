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
        void writeToBlock(const std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml)
        {
            try
            {
                auto materialsUUID = std::any_cast<std::vector<std::shared_ptr<UUID>>>(metadata->getValue("textures"));
                std::vector<std::string> materials;
                for (const auto &uuid: materialsUUID)
                {
                    materials.push_back(uuid->toString());
                }
                yml->set("MaterialBlockOperator.textures", materials);
            } catch (const std::bad_any_cast &)
            {
                yml->set("MaterialBlockOperator.textures", std::vector<std::string>{});
            }

            auto getVec3 = [&](const std::string &key, const glm::vec3 &defaultValue) -> glm::vec3
            {
                try
                {
                    return std::any_cast<glm::vec3>(metadata->getValue(key));
                } catch (const std::bad_any_cast &)
                {
                    return defaultValue;
                }
            };

            glm::vec3 diffuse = getVec3("diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
            glm::vec3 specular = getVec3("specular", glm::vec3(1.0f, 1.0f, 1.0f));
            glm::vec3 ambient = getVec3("ambient", glm::vec3(1.0f, 1.0f, 1.0f));
            glm::vec3 emissive = getVec3("emissive", glm::vec3(1.0f, 1.0f, 1.0f));

            yml->set("MaterialBlockOperator.diffuse.x", diffuse.x);
            yml->set("MaterialBlockOperator.diffuse.y", diffuse.y);
            yml->set("MaterialBlockOperator.diffuse.z", diffuse.z);
            yml->set("MaterialBlockOperator.specular.x", specular.x);
            yml->set("MaterialBlockOperator.specular.y", specular.y);
            yml->set("MaterialBlockOperator.specular.z", specular.z);
            yml->set("MaterialBlockOperator.ambient.x", ambient.x);
            yml->set("MaterialBlockOperator.ambient.y", ambient.y);
            yml->set("MaterialBlockOperator.ambient.z", ambient.z);
            yml->set("MaterialBlockOperator.emissive.x", emissive.x);
            yml->set("MaterialBlockOperator.emissive.y", emissive.y);
            yml->set("MaterialBlockOperator.emissive.z", emissive.z);

            auto getFloat = [&](const std::string &key, float defaultValue) -> float
            {
                try
                {
                    return std::any_cast<float>(metadata->getValue(key));
                } catch (const std::bad_any_cast &)
                {
                    return defaultValue;
                }
            };

            yml->set("MaterialBlockOperator.shininess", getFloat("shininess", 0.5f));
            yml->set("MaterialBlockOperator.opacity", getFloat("opacity", 1.0f));
            yml->set("MaterialBlockOperator.roughness", getFloat("roughness", 0.5f));
            yml->set("MaterialBlockOperator.metallic", getFloat("metallic", 0.5f));
            yml->set("MaterialBlockOperator.reflectivity", getFloat("reflectivity", 0.5f));
        }

        void readFromBlock(const std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml)
        {
            auto materialsVec = yml->getStringList("MaterialBlockOperator.textures");
            std::vector<std::shared_ptr<UUID>> materialsUUID;
            materialsUUID.reserve(materialsVec.size());
            for (const auto &materialStr: materialsVec)
            {
                materialsUUID.push_back(UUIDManager::getUUID(materialStr));
            }
            metadata->setValue("textures", materialsUUID);

            glm::vec3 defaultColor(0.0f, 0.0f, 0.0f);
            float defaultFloat = 0.0f;

            glm::vec3 diffuse(
                    yml->getFloat("MaterialBlockOperator.diffuse.x", defaultColor.x),
                    yml->getFloat("MaterialBlockOperator.diffuse.y", defaultColor.y),
                    yml->getFloat("MaterialBlockOperator.diffuse.z", defaultColor.z)
            );
            glm::vec3 specular(
                    yml->getFloat("MaterialBlockOperator.specular.x", defaultColor.x),
                    yml->getFloat("MaterialBlockOperator.specular.y", defaultColor.y),
                    yml->getFloat("MaterialBlockOperator.specular.z", defaultColor.z)
            );
            glm::vec3 ambient(
                    yml->getFloat("MaterialBlockOperator.ambient.x", defaultColor.x),
                    yml->getFloat("MaterialBlockOperator.ambient.y", defaultColor.y),
                    yml->getFloat("MaterialBlockOperator.ambient.z", defaultColor.z)
            );
            glm::vec3 emissive(
                    yml->getFloat("MaterialBlockOperator.emissive.x", defaultColor.x),
                    yml->getFloat("MaterialBlockOperator.emissive.y", defaultColor.y),
                    yml->getFloat("MaterialBlockOperator.emissive.z", defaultColor.z)
            );

            metadata->setValue("diffuse", diffuse);
            metadata->setValue("specular", specular);
            metadata->setValue("ambient", ambient);
            metadata->setValue("emissive", emissive);

            metadata->setValue("shininess", yml->getFloat("MaterialBlockOperator.shininess", defaultFloat));
            metadata->setValue("opacity", yml->getFloat("MaterialBlockOperator.opacity", 1.0f));
            metadata->setValue("roughness", yml->getFloat("MaterialBlockOperator.roughness", defaultFloat));
            metadata->setValue("metallic", yml->getFloat("MaterialBlockOperator.metallic", defaultFloat));
            metadata->setValue("reflectivity", yml->getFloat("MaterialBlockOperator.reflectivity", defaultFloat));
        }

        void initBlock(std::shared_ptr<Metadata> &metadata, const std::vector<std::any> &params)
        {
            glm::vec3 defaultColor(0.5f, 0.5f, 0.5f);
            float defaultFloat = 0.5f;
            std::vector<std::string> defaultTextures;

            if (params.size() > 0 && params[0].type() == typeid(std::vector<std::string>))
            {
                std::vector<std::string> materials = std::any_cast<std::vector<std::string>>(params[0]);
                std::vector<std::shared_ptr<UUID>> materialsUUID;
                materialsUUID.reserve(materials.size());
                for (const auto &material: materials)
                {
                    materialsUUID.push_back(UUIDManager::getUUID(material));
                }
                metadata->setValue("textures", materialsUUID);
            }
            else metadata->setValue("textures", defaultTextures);

            // Check and set vec3 properties
            glm::vec3 diffuse = params.size() > 1 && params[1].type() == typeid(glm::vec3) ? std::any_cast<glm::vec3>(params[1])
                                                                                           : defaultColor;
            glm::vec3 specular = params.size() > 2 && params[2].type() == typeid(glm::vec3) ? std::any_cast<glm::vec3>(params[2])
                                                                                            : defaultColor;
            glm::vec3 ambient = params.size() > 3 && params[3].type() == typeid(glm::vec3) ? std::any_cast<glm::vec3>(params[3])
                                                                                           : defaultColor;
            glm::vec3 emissive = params.size() > 4 && params[4].type() == typeid(glm::vec3) ? std::any_cast<glm::vec3>(params[4])
                                                                                            : defaultColor;

            metadata->setValue("diffuse", diffuse);
            metadata->setValue("specular", specular);
            metadata->setValue("ambient", ambient);
            metadata->setValue("emissive", emissive);

            float shininess = params.size() > 5 && params[5].type() == typeid(float) ? std::any_cast<float>(params[5])
                                                                                     : defaultFloat;
            float opacity = params.size() > 6 && params[6].type() == typeid(float) ? std::any_cast<float>(params[6])
                                                                                   : 1.0f;
            float roughness = params.size() > 7 && params[7].type() == typeid(float) ? std::any_cast<float>(params[7])
                                                                                     : defaultFloat;
            float metallic = params.size() > 8 && params[8].type() == typeid(float) ? std::any_cast<float>(params[8])
                                                                                    : defaultFloat;
            float reflectivity =
                    params.size() > 9 && params[9].type() == typeid(float) ? std::any_cast<float>(params[9])
                                                                           : defaultFloat;

            metadata->setValue("shininess", shininess);
            metadata->setValue("opacity", opacity);
            metadata->setValue("roughness", roughness);
            metadata->setValue("metallic", metallic);
            metadata->setValue("reflectivity", reflectivity);
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
        explicit Material(const std::shared_ptr<base::UUID> &existingUuid, bool init,
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
                  IMetaAccessor(yml, init, existingUuid),
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

        std::shared_ptr<Texture> getRenderTexture(const TextureType type)
        {
            auto it = textures.find(type);
            if (it != textures.end())
            {
                if (!it->second.empty())
                {
                    return it->second.begin()->second;
                }
            }
            return nullptr;
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