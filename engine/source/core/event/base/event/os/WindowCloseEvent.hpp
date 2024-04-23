//
// Created by Demon on 2024/4/23.
//

#ifndef DEMONENGINE_WINDOWCLOSEEVENT_HPP
#define DEMONENGINE_WINDOWCLOSEEVENT_HPP

#include "core/base/interface/Interface.hpp"
#include "core/event/interface/IEvent.hpp"

namespace event::base
{
    class WindowCloseEvent : implements IEvent
    {
    private:
        static const std::string eventName;
    public:
        WindowCloseEvent() : IEvent()
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
    };
}

#endif //DEMONENGINE_WINDOWCLOSEEVENT_HPP
