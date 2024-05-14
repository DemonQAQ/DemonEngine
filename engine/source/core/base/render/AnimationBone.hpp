//
// Created by Demon on 2024/2/28.
//

#ifndef DEMONENGINE_ANIMATIONBONE_HPP
#define DEMONENGINE_ANIMATIONBONE_HPP

#include "glm/ext/matrix_float4x4.hpp"
#include "ModelBone.hpp"
#include <string>
#include <utility>
#include <vector>
#include <iostream>

namespace base
{
    struct KeyPosition
    {
        glm::vec3 position;
        float timeStamp;
    };

    struct KeyRotation
    {
        glm::quat orientation;
        float timeStamp;
    };

    struct KeyScale
    {
        glm::vec3 scale;
        float timeStamp;
    };

    enum class InterpolationType
    {
        LINEAR,
        SPLINE,
        CUBIC
    };

    class AnimationBone : implements INameable
    {
    private:
        std::string name;
        int id;
        std::vector<KeyPosition> keyPositions;
        std::vector<KeyRotation> keyRotations;
        std::vector<KeyScale> keyScales;
        int keyNumPositions = -1;
        int keyNumRotations = -1;
        int keyNumScalings = -1;
        float lastTimeStamp = -1;
        bool upend = false;
        float speed = 1;
        Transform transform;
        InterpolationType interpolationType;
    public:

        AnimationBone(std::string name, int id, InterpolationType type = InterpolationType::LINEAR) :
                name(std::move(name)), id(id), interpolationType(type)
        {}

        [[nodiscard]] int getId() const
        {
            return id;
        }

        void setSpeed(float speed_)
        {
            speed = speed_;
        }

        float getSpeed()
        {
            return speed;
        }

        void setUpend(bool upend_)
        {
            upend = upend_;
        }

        bool isUpend()
        {
            return upend;
        }

        Transform &getTransform()
        {
            return transform;
        }

        void updateAnimation(float deltaTime)
        {
            float adjustedTime = lastTimeStamp + deltaTime * speed * (upend ? -1.0f : 1.0f);
            lastTimeStamp = adjustedTime;

            transform.position = interpolatePosition(keyPositions, adjustedTime, keyNumPositions);
            transform.rotation = interpolateRotation(keyRotations, adjustedTime, keyNumRotations);
            transform.scale = interpolateScale(keyScales, adjustedTime, keyNumScalings);
        }

        void start(float nowTimeStamp)
        {
            reset();
            lastTimeStamp = nowTimeStamp;
        }

        void reset()
        {
            keyNumPositions = 0;
            keyNumRotations = 0;
            keyNumScalings = 0;
        }

        void setName(const std::string &name_) override
        {
            name = name_;
        }

        [[nodiscard]] std::string getName() const override
        {
            return name;
        }

        glm::vec3 interpolatePosition(const std::vector<KeyPosition> &positions, float timeStamp, int &currentIndex)
        {
            if (positions.empty())
                return glm::vec3(0.0f);

            size_t index = std::min(static_cast<size_t>(currentIndex), positions.size() - 1);
            while (index < positions.size() - 1 && timeStamp > positions[index + 1].timeStamp)
                ++index;
            while (index > 0 && timeStamp < positions[index].timeStamp)
                --index;

            currentIndex = index;
            if (timeStamp <= positions.front().timeStamp)
                return positions.front().position;
            if (timeStamp >= positions.back().timeStamp)
                return positions.back().position;

            const auto &start = positions[index];
            const auto &end = positions[index + 1];
            float t = (timeStamp - start.timeStamp) / (end.timeStamp - start.timeStamp);
            std::vector<glm::vec3> points;
            for (const auto &keyPos: positions)
            {
                points.push_back(keyPos.position);
            }

            switch (interpolationType)
            {
                case InterpolationType::LINEAR:
                    return glm::mix(start.position, end.position, t);
                case InterpolationType::SPLINE:
                    return catmullRomSpline(points, index, t);
                case InterpolationType::CUBIC:
                    return cubicBezier(points, index, t);
                default:
                    return glm::mix(start.position, end.position, t); // 默认线性插值
            }
        }

        glm::quat interpolateRotation(const std::vector<KeyRotation> &rotations, float timeStamp, int &currentIndex)
        {
            if (rotations.empty())
                return {};

            size_t index = std::min(static_cast<size_t>(currentIndex), rotations.size() - 1);
            while (index < rotations.size() - 1 && timeStamp > rotations[index + 1].timeStamp)
                ++index;
            while (index > 0 && timeStamp < rotations[index].timeStamp)
                --index;

            currentIndex = index;
            if (timeStamp <= rotations.front().timeStamp)
                return rotations.front().orientation;
            if (timeStamp >= rotations.back().timeStamp)
                return rotations.back().orientation;

            const auto &start = rotations[index];
            const auto &end = rotations[index + 1];
            float t = (timeStamp - start.timeStamp) / (end.timeStamp - start.timeStamp);
            std::vector<glm::quat> points;
            for (const auto &keyPos: rotations)
            {
                points.push_back(keyPos.orientation);
            }

            switch (interpolationType)
            {
                case InterpolationType::LINEAR:
                    return glm::mix(start.orientation, end.orientation, t);
                case InterpolationType::SPLINE:
                    return catmullRomSpline(points, index, t);
                case InterpolationType::CUBIC:
                    return cubicBezier(points, index, t);
                default:
                    return glm::mix(start.orientation, end.orientation, t); // 默认线性插值
            }
        }

        glm::vec3 interpolateScale(const std::vector<KeyScale> &scales, float timeStamp, int &currentIndex)
        {
            if (scales.empty())
                return glm::vec3(1.0f);

            size_t index = std::min(static_cast<size_t>(currentIndex), scales.size() - 1);
            while (index < scales.size() - 1 && timeStamp > scales[index + 1].timeStamp)
                ++index;
            while (index > 0 && timeStamp < scales[index].timeStamp)
                --index;

            currentIndex = index;
            if (timeStamp <= scales.front().timeStamp)
                return scales.front().scale;
            if (timeStamp >= scales.back().timeStamp)
                return scales.back().scale;

            const auto &start = scales[index];
            const auto &end = scales[index + 1];
            float t = (timeStamp - start.timeStamp) / (end.timeStamp - start.timeStamp);
            std::vector<glm::vec3> points;
            for (const auto &keyPos: scales)
            {
                points.push_back(keyPos.scale);
            }

            switch (interpolationType)
            {
                case InterpolationType::LINEAR:
                    return glm::mix(start.scale, end.scale, t);
                case InterpolationType::SPLINE:
                    return catmullRomSpline(points, index, t);
                case InterpolationType::CUBIC:
                    return cubicBezier(points, index, t);
                default:
                    return glm::mix(start.scale, end.scale, t); // 默认线性插值
            }
        }

        glm::vec3 catmullRomSpline(const std::vector<glm::vec3> &points, size_t index, float t)
        {
            if (index == 0 || index + 2 >= points.size())
            {
                // 边界条件，简单使用线性插值
                return glm::mix(points[index], points[index + 1], t);
            }

            const glm::vec3 &p0 = points[index - 1];
            const glm::vec3 &p1 = points[index];
            const glm::vec3 &p2 = points[index + 1];
            const glm::vec3 &p3 = points[index + 2];

            float t2 = t * t;
            float t3 = t2 * t;

            float a = -0.5 * t3 + t2 - 0.5 * t;
            float b = 1.5 * t3 - 2.5 * t2 + 1.0;
            float c = -1.5 * t3 + 2.0 * t2 + 0.5 * t;
            float d = 0.5 * t3 - 0.5 * t2;

            return a * p0 + b * p1 + c * p2 + d * p3;
        }

        glm::vec3 cubicBezier(const std::vector<glm::vec3> &points, size_t index, float t)
        {
            if (index + 3 > points.size())
            {
                // 不足以构成三次贝塞尔曲线
                return glm::mix(points[index], points[index + 1], t);
            }

            const glm::vec3 &p0 = points[index];
            const glm::vec3 &p1 = p0 + (points[index + 1] - p0) * 0.33f;  // 第一个控制点
            const glm::vec3 &p2 = points[index + 1] + (points[index + 2] - points[index + 1]) * 0.33f; // 第二个控制点
            const glm::vec3 &p3 = points[index + 1];

            float t2 = t * t;
            float t3 = t2 * t;

            return (1 - t) * (1 - t) * (1 - t) * p0 +
                   3 * (1 - t) * (1 - t) * t * p1 +
                   3 * (1 - t) * t * t * p2 +
                   t3 * p3;
        }

        glm::quat catmullRomSpline(const std::vector<glm::quat> &quats, size_t index, float t)
        {
            if (index == 0 || index + 2 >= quats.size())
            {
                // 边界条件，简单使用 SLERP
                return glm::slerp(quats[index], quats[index + 1], t);
            }

            glm::quat q0 = quats[index - 1];
            glm::quat q1 = quats[index];
            glm::quat q2 = quats[index + 1];
            glm::quat q3 = quats[index + 2];

            // 手动模拟squad插值，使用slerp的组合
            glm::quat s1 = glm::slerp(q0, q3, t); // 首先外插
            glm::quat s2 = glm::slerp(q1, q2, t); // 内插

            return glm::slerp(s1, s2, t); // 结合两次插值的结果
        }

        glm::quat cubicBezier(const std::vector<glm::quat> &quats, size_t index, float t)
        {
            if (index + 3 > quats.size())
            {
                // 不足以构成三次贝塞尔曲线
                return glm::slerp(quats[index], quats[index + 1], t);
            }

            glm::quat q0 = quats[index];
            glm::quat q1 = glm::slerp(q0, quats[index + 1], 0.33f); // 第一个控制点
            glm::quat q2 = glm::slerp(quats[index + 1], quats[index + 2], 0.33f); // 第二个控制点
            glm::quat q3 = quats[index + 1];

            // 对四元数进行三次插值
            glm::quat s1 = glm::slerp(q0, q1, t);
            glm::quat s2 = glm::slerp(q1, q2, t);
            glm::quat s3 = glm::slerp(q2, q3, t);

            glm::quat s4 = glm::slerp(s1, s2, t);
            glm::quat s5 = glm::slerp(s2, s3, t);

            return glm::slerp(s4, s5, t);
        }
    };
}

#endif //DEMONENGINE_ANIMATIONBONE_HPP
