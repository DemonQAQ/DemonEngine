//
// Created by Demon on 2024/3/5.
//

#ifndef DEMONENGINE_INAMEABLE_HPP
#define DEMONENGINE_INAMEABLE_HPP

#include <string>

namespace base
{
    class INameable
    {
    public:
        virtual void setName(const std::string &name_) = 0;

        [[nodiscard]] virtual std::string getName() const = 0;
    };
}

#endif //DEMONENGINE_INAMEABLE_HPP
