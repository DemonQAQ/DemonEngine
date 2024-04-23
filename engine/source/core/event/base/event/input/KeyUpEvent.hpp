//
// Created by Demon on 2024/4/23.
//

#ifndef DEMONENGINE_KEYUPEVENT_HPP
#define DEMONENGINE_KEYUPEVENT_HPP

#include "core/base/interface/Interface.hpp"
#include "core/event/interface/IEvent.hpp"
#include "KeyType.hpp"

namespace event::base
{
    class KeyUpEvent : implements IEvent
    {
    private:
        KeyType keyType;
        static const std::string eventName;
    public:
        explicit KeyUpEvent(KeyType keyType_)
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

#endif //DEMONENGINE_KEYUPEVENT_HPP
