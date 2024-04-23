//
// Created by Demon on 2024/4/23.
//

#ifndef DEMONENGINE_MOUSEMOVEEVENT_HPP
#define DEMONENGINE_MOUSEMOVEEVENT_HPP

#include "core/base/interface/Interface.hpp"
#include "core/event/interface/IEvent.hpp"

namespace event::base
{
    class MouseMoveEvent : implements IEvent
    {
    private:
        static const std::string eventName;
        double xPos;
        double yPos;
    public:
        MouseMoveEvent(double xPos_, double yPos_) : IEvent(), xPos(xPos_), yPos(yPos_)
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

        [[nodiscard]] double getXPos() const
        {
            return xPos;
        }

        [[nodiscard]] double getYPos() const
        {
            return yPos;
        }
    };
}

#endif //DEMONENGINE_MOUSEMOVEEVENT_HPP
