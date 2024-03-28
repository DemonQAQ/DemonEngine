﻿//
// Created by Demon on 2024/3/27.
//

#ifndef DEMONENGINE_EFFECT_HPP
#define DEMONENGINE_EFFECT_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"
#include "core/base/interface/ITransformableUpdate.hpp"

using namespace base;

namespace assets::scene
{

    class Effect : implements Object, INameable, ITransformableUpdate
    {
    private:
        std::string name;
    public:
        explicit Effect(std::string name = "Effect Entity");

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        void beforeRendering(const std::vector<std::any> &params) override;

        void afterRendering(const std::vector<std::any> &params) override;
    };

} // assets

#endif //DEMONENGINE_EFFECT_HPP
