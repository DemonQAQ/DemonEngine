//
// Created by Demon on 2024/3/24.
//

#ifndef DEMONENGINE_LIGHT_HPP
#define DEMONENGINE_LIGHT_HPP

#include "glm/glm.hpp"

namespace base
{
    enum class LightType
    {
        POINT,
        DIRECTIONAL,
        SPOTLIGHT
    };

    struct Light
    {
        LightType type;       // 光源类型
        glm::vec3 position;   // 光源位置，对于定向光源，这个可以表示方向
        glm::vec3 color;      // 光源颜色
        float intensity;      // 光源强度

        // 点光源和聚光灯特有属性
        float constant;       // 常数衰减
        float linear;         // 线性衰减
        float quadratic;      // 二次衰减

        // 聚光灯特有属性
        glm::vec3 direction;  // 聚光灯方向
        float cutOff;         // 聚光灯截断角度
        float outerCutOff;    // 聚光灯外截断角度

        Light()
                : type(LightType::POINT),
                  position(glm::vec3(0.0f)),
                  color(glm::vec3(1.0f)),
                  intensity(1.0f),
                  constant(1.0f),
                  linear(0.09f),
                  quadratic(0.032f),
                  direction(glm::vec3(0.0f, -1.0f, 0.0f)),
                  cutOff(glm::cos(glm::radians(12.5f))),
                  outerCutOff(glm::cos(glm::radians(15.0f)))
        {}

        // 点光源构造函数
        Light(const glm::vec3 &pos, const glm::vec3 &col, float intens, float constAtt, float linAtt, float quadAtt)
                : type(LightType::POINT),
                  position(pos),
                  color(col),
                  intensity(intens),
                  constant(constAtt),
                  linear(linAtt),
                  quadratic(quadAtt),
                  direction(glm::vec3(0.0f)),
                  cutOff(0.0f),
                  outerCutOff(0.0f)
        {}

        // 定向光源构造函数
        Light(const glm::vec3 &dir, const glm::vec3 &col, float intens)
                : type(LightType::DIRECTIONAL),
                  position(glm::vec3(0.0f)), // 对定向光源而言，position不适用
                  color(col),
                  intensity(intens),
                  constant(1.0f),
                  linear(0.0f),
                  quadratic(0.0f),
                  direction(dir),
                  cutOff(0.0f),
                  outerCutOff(0.0f)
        {}

        // 聚光灯构造函数
        Light(const glm::vec3 &pos, const glm::vec3 &dir, const glm::vec3 &col, float intens, float constAtt,
              float linAtt, float quadAtt, float co, float outerCo)
                : type(LightType::SPOTLIGHT),
                  position(pos),
                  color(col),
                  intensity(intens),
                  constant(constAtt),
                  linear(linAtt),
                  quadratic(quadAtt),
                  direction(dir),
                  cutOff(glm::cos(glm::radians(co))),
                  outerCutOff(glm::cos(glm::radians(outerCo)))
        {}

        // 便捷构造函数等可根据需要添加
    };

}


#endif //DEMONENGINE_LIGHT_HPP
