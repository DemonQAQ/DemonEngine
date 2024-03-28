//
// Created by Demon on 2024/3/27.
//

#include "LightEntity.hpp"

#include <utility>

namespace assets::scene
{
    LightEntity::LightEntity(const std::string &uuidStr, const base::Light &light_, std::string name) : base::Object(
            uuidStr), name(std::move(name)), light(light_)
    {

    }

    LightEntity::LightEntity(std::string name) : base::Object(), name(std::move(name))
    {

    }

    void LightEntity::setName(const std::string &name_)
    {
        name = name_;
    }

    std::string LightEntity::getName() const
    {
        return name;
    }

    void LightEntity::beforeRendering(const std::vector<std::any> &params)
    {

    }

    void LightEntity::afterRendering(const std::vector<std::any> &params)
    {

    }

    void LightEntity::setLightType(base::LightType type)
    {
        light.type = type;
    }

    [[nodiscard]] base::LightType LightEntity::getLightType() const
    {
        return light.type;
    }

    void LightEntity::setLightPosition(const glm::vec3 &position)
    {
        light.position = position;
    }

    [[nodiscard]] glm::vec3 LightEntity::getLightPosition() const
    {
        return light.position;
    }

    void LightEntity::setColor(const glm::vec3 &color)
    {
        light.color = color;
    }

    [[nodiscard]] glm::vec3 LightEntity::getColor() const
    {
        return light.color;
    }

    void LightEntity::setIntensity(float intensity)
    {
        light.intensity = intensity;
    }

    [[nodiscard]] float LightEntity::getIntensity() const
    {
        return light.intensity;
    }

    void LightEntity::setAttenuation(float constant, float linear, float quadratic)
    {
        light.constant = constant;
        light.linear = linear;
        light.quadratic = quadratic;
    }

    [[nodiscard]] float LightEntity::getConstantAttenuation() const
    {
        return light.constant;
    }

    [[nodiscard]] float LightEntity::getLinearAttenuation() const
    {
        return light.linear;
    }

    [[nodiscard]] float LightEntity::getQuadraticAttenuation() const
    {
        return light.quadratic;
    }

    void LightEntity::setDirection(const glm::vec3 &direction)
    {
        light.direction = direction;
    }

    [[nodiscard]] glm::vec3 LightEntity::getDirection() const
    {
        return light.direction;
    }

    void LightEntity::setCutOff(float cutOff)
    {
        light.cutOff = glm::cos(glm::radians(cutOff));
    }

    [[nodiscard]] float LightEntity::getCutOff() const
    {
        return glm::degrees(glm::acos(light.cutOff));
    }

    void LightEntity::setOuterCutOff(float outerCutOff)
    {
        light.outerCutOff = glm::cos(glm::radians(outerCutOff));
    }

    [[nodiscard]] float LightEntity::getOuterCutOff() const
    {
        return glm::degrees(glm::acos(light.outerCutOff));
    }


}