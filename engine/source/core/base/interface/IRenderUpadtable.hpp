//
// Created by Demon on 2024/3/31.
//

#ifndef DEMONENGINE_IRENDERUPADTABLE_HPP
#define DEMONENGINE_IRENDERUPADTABLE_HPP

#include "Interface.hpp"
#include <string>
#include <vector>
#include <any>

namespace base
{
    interface IRenderUpdatable
    {
    public:
        virtual void beforeRendering(const std::vector <std::any> &params) = 0;

        virtual void afterRendering(const std::vector <std::any> &params) = 0;
    };
}

#endif //DEMONENGINE_IRENDERUPADTABLE_HPP
