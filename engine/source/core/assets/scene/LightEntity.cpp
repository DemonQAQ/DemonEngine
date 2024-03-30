//
// Created by Demon on 2024/3/27.
//

#include "LightEntity.hpp"
#include <sstream>
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

    std::string LightEntity::Serialize() const
    {
        boost::property_tree::ptree pt;

        // 基本属性
        pt.put("name", name);
        pt.put("light.type", static_cast<int>(light.type));
        pt.put("light.position", std::to_string(light.position.x) + " " +
                                 std::to_string(light.position.y) + " " +
                                 std::to_string(light.position.z));
        pt.put("light.color", std::to_string(light.color.x) + " " +
                              std::to_string(light.color.y) + " " +
                              std::to_string(light.color.z));
        pt.put("light.intensity", light.intensity);

        // 衰减属性
        pt.put("light.constant", light.constant);
        pt.put("light.linear", light.linear);
        pt.put("light.quadratic", light.quadratic);

        // 聚光灯属性
        pt.put("light.direction", std::to_string(light.direction.x) + " " +
                                  std::to_string(light.direction.y) + " " +
                                  std::to_string(light.direction.z));
        pt.put("light.cutOff", light.cutOff);
        pt.put("light.outerCutOff", light.outerCutOff);

        std::ostringstream buf;
        boost::property_tree::write_json(buf, pt, true);
        return buf.str();
    }

    void LightEntity::Deserialize(const std::string &data)
    {
        std::istringstream is(data);
        boost::property_tree::ptree pt;
        boost::property_tree::read_json(is, pt);

        name = pt.get<std::string>("name");
        light.type = static_cast<LightType>(pt.get<int>("light.type"));

        std::istringstream positionStream(pt.get<std::string>("light.position"));
        positionStream >> light.position.x >> light.position.y >> light.position.z;

        std::istringstream colorStream(pt.get<std::string>("light.color"));
        colorStream >> light.color.x >> light.color.y >> light.color.z;

        light.intensity = pt.get<float>("light.intensity");

        light.constant = pt.get<float>("light.constant");
        light.linear = pt.get<float>("light.linear");
        light.quadratic = pt.get<float>("light.quadratic");

        std::istringstream directionStream(pt.get<std::string>("light.direction"));
        directionStream >> light.direction.x >> light.direction.y >> light.direction.z;

        light.cutOff = pt.get<float>("light.cutOff");
        light.outerCutOff = pt.get<float>("light.outerCutOff");
    }

}