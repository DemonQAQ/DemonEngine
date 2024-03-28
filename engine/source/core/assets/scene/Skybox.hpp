//
// Created by Demon on 2024/3/27.
//

#ifndef DEMONENGINE_SKYBOX_HPP
#define DEMONENGINE_SKYBOX_HPP

#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"

using namespace base;

namespace assets::scene
{
    class Skybox : implements Object, INameable
    {
    private:
        std::string name;
    public:
        explicit Skybox(std::string name = "Skybox");

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;
    };
}


#endif //DEMONENGINE_SKYBOX_HPP
