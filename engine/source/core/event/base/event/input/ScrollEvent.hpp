//
// Created by Demon on 2024/4/23.
//

#ifndef DEMONENGINE_SCROLLEVENT_HPP
#define DEMONENGINE_SCROLLEVENT_HPP

#include "core/base/interface/Interface.hpp"
#include "core/event/interface/IEvent.hpp"

namespace event::base
{
    class ScrollEvent : implements IEvent
    {
    private:
        static const std::string eventName;
        double xOffset;
        double yOffset;
    public:
        ScrollEvent(double xOffset, double yOffset) : IEvent(), xOffset(xOffset), yOffset(yOffset)
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

        double getXOffset() const
        {
            return xOffset;
        }

        double getYOffset() const
        {
            return yOffset;
        }
    };
}


#endif //DEMONENGINE_SCROLLEVENT_HPP
