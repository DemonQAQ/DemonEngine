//
// Created by Demon on 2024/4/23.
//

#ifndef DEMONENGINE_WINDOWSIZECHANGEEVENT_HPP
#define DEMONENGINE_WINDOWSIZECHANGEEVENT_HPP

#include "core/base/interface/Interface.hpp"
#include "core/event/interface/IEvent.hpp"

namespace event::base
{
    class WindowSizeChangeEvent : implements IEvent
    {
    private:
        static const std::string eventName;
        int width;
        int height;
    public:
        WindowSizeChangeEvent(int width_, int height_) : IEvent(), width(width_), height(height_)
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

        int getWidth() const
        {
            return width;
        }

        int getHeight() const
        {
            return height;
        }
    };
}

#endif //DEMONENGINE_WINDOWSIZECHANGEEVENT_HPP
