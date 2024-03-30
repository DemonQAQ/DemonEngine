//
// Created by Demon on 2024/3/27.
//

#ifndef DEMONENGINE_LIGHTENTITY_HPP
#define DEMONENGINE_LIGHTENTITY_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"
#include "core/base/render/Light.hpp"
#include "core/io/interface/ISerializable.hpp"


using namespace base;

namespace assets::scene
{
    class LightEntity : implements Object, INameable, ITransformableUpdate, io::ISerializable
    {
    private:
        std::string name;
        Light light;
    public:
        explicit LightEntity(std::string name = "Normal Light");

        LightEntity(const std::string &uuidStr, const base::Light &light_,std::string name = "Normal Light");

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        void beforeRendering(const std::vector<std::any> &params) override;

        void afterRendering(const std::vector<std::any> &params) override;

        [[nodiscard]]  std::string Serialize() const override;

        void Deserialize(const std::string &data) override;

        void setLightType(base::LightType type);

        [[nodiscard]] base::LightType getLightType() const;

        void setLightPosition(const glm::vec3 &position);

        [[nodiscard]] glm::vec3 getLightPosition() const;

        void setColor(const glm::vec3 &color);

        [[nodiscard]] glm::vec3 getColor() const;

        void setIntensity(float intensity);

        [[nodiscard]] float getIntensity() const;

        // 设置衰减参数
        void setAttenuation(float constant, float linear, float quadratic);

        // 获取常数衰减
        [[nodiscard]] float getConstantAttenuation() const;

        // 获取线性衰减
        [[nodiscard]] float getLinearAttenuation() const;

        // 获取二次衰减
        [[nodiscard]] float getQuadraticAttenuation() const;

        // 设置聚光灯方向
        void setDirection(const glm::vec3 &direction);

        // 获取聚光灯方向
        [[nodiscard]] glm::vec3 getDirection() const;

        // 设置聚光灯截断角度
        void setCutOff(float cutOff);

        // 获取聚光灯截断角度
        [[nodiscard]] float getCutOff() const;

        // 设置聚光灯外截断角度
        void setOuterCutOff(float outerCutOff);

        // 获取聚光灯外截断角度
        [[nodiscard]] float getOuterCutOff() const;
    };
}


#endif //DEMONENGINE_LIGHTENTITY_HPP
