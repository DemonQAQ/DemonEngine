//
// Created by Demon on 2024/3/28.
//

#ifndef DEMONENGINE_IUPDATABLE_HPP
#define DEMONENGINE_IUPDATABLE_HPP

#include "Interface.hpp"

namespace base
{
    interface Updatable
    {
    public:
        virtual void update() = 0;
    };
}

#endif //DEMONENGINE_IUPDATABLE_HPP
