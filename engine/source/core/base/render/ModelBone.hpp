//
// Created by Demon on 2024/5/15.
//

#ifndef DEMONENGINE_MODELBONE_HPP
#define DEMONENGINE_MODELBONE_HPP

#include <string>
#include <utility>
#include "core/base/common/Transform.hpp"
#include "core/base/interface/Interface.hpp"
#include "core/base/interface/IRenderable.hpp"

namespace base
{
    struct BoneInfo
    {
        int id;
        Transform finalTransformation;

        BoneInfo(int id_, const Transform &transform_) : id(id_), finalTransformation(transform_)
        {}

        BoneInfo(int id_, const glm::mat4 &transform_) : id(id_), finalTransformation(transform_)
        {}
    };

    class ModelBone : implements INameable
    {
    private:
        std::string name;
        std::shared_ptr<std::vector<ModelBone>> children;
        std::shared_ptr<BoneInfo> boneInfo;
        Transform transform;
    public:
        ModelBone(std::string name, int id, const Transform &transform,
                  const std::shared_ptr<std::vector<ModelBone>> &children = nullptr) :
                name(std::move(name)), children(children), boneInfo(std::make_shared<BoneInfo>(id, transform))
        {

        }

        void setName(const std::string &name_) override
        {
            this->name = name_;
        }

        [[nodiscard]] std::string getName() const override
        {
            return name;
        }

        void addBone(const ModelBone &bone)
        {
            children->push_back(bone);
        }

        [[nodiscard]] const std::shared_ptr<BoneInfo> &getBoneInfo() const
        {
            return boneInfo;
        }

        [[nodiscard]] const std::shared_ptr<std::vector<ModelBone>> &getChildren() const
        {
            return children;
        }
    };
}
#endif //DEMONENGINE_MODELBONE_HPP
