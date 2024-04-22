//
// Created by Demon on 2024/4/23.
//

#ifndef DEMONENGINE_IEVENT_HPP
#define DEMONENGINE_IEVENT_HPP

#include <string>
#include "core/base/interface/Interface.hpp"

namespace event
{
    interface IEvent
    {
    private:
        bool isCancel_ = false;
    public:
        virtual std::string getEventName() const = 0;

        [[nodiscard]] bool isCancel() const
        {
            return isCancel_;
        }

        void setCancel(bool value)
        {
            isCancel_ = value;
        }
    };
}
#endif //DEMONENGINE_IEVENT_HPP
