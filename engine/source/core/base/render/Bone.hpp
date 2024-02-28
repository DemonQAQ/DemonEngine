//
// Created by Demon on 2024/2/28.
//

#ifndef DEMONENGINE_BONE_HPP
#define DEMONENGINE_BONE_HPP

#include "glm/ext/matrix_float4x4.hpp"
#include <string>
#include <utility>
#include <vector>
#include <iostream>

namespace base
{
    struct BoneInfo
    {
        glm::mat4 boneOffset;
        glm::mat4 finalTransformation;

        BoneInfo() : boneOffset(glm::mat4(1.0f)), finalTransformation(glm::mat4(1.0f))
        {}
    };

    class Bone
    {
    public:
        std::string name;
        std::vector <Bone> children;
        BoneInfo info;

        Bone(std::string name, const BoneInfo &info) : name(std::move(name)), info(info)
        {}
    };
}

#endif //DEMONENGINE_BONE_HPP
