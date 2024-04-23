//
// Created by Demon on 2024/4/23.
//

#ifndef DEMONENGINE_KEYDOWNEVENT_HPP
#define DEMONENGINE_KEYDOWNEVENT_HPP

#include "core/base/interface/Interface.hpp"
#include "core/event/interface/IEvent.hpp"
#include "KeyType.hpp"

namespace event::base
{
    class KeyDownEvent : implements IEvent
    {
    private:
        KeyType keyType;
        static const std::string eventName;
    public:
        explicit KeyDownEvent(KeyType keyType_)
                : IEvent(), keyType(keyType_)
        {
        }

        [[nodiscard]] std::string getEventName() const override
        {
            return eventName;
        }

        [[nodiscard]] static std::string staticGetEventName()
        {
            return eventName;
        }

        [[nodiscard]] KeyType getKeyType() const
        {
            return keyType;
        }
    };
}


#endif //DEMONENGINE_KEYDOWNEVENT_HPP
