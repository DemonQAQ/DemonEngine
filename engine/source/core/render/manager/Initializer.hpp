//
// Created by Demon on 2024/3/25.
//

#ifndef DEMONENGINE_INITIALIZER_HPP
#define DEMONENGINE_INITIALIZER_HPP

#include <vector>
#include <any>
#include "core/base/interface/Interface.hpp"

namespace render
{
    interface Initializer
    {
    private:
        bool inited;
    public:
        Initializer() : inited(false)
        {};

        virtual bool init(const std::vector<std::any> &params) = 0;

        [[nodiscard]] bool isInit() const
        {
            return inited;
        }

    protected:
        void setInit(bool flag)
        {
            inited = flag;
        }
    };
}

#endif //DEMONENGINE_INITIALIZER_HPP
