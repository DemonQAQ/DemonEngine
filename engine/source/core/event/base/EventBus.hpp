//
// Created by Demon on 2024/4/23.
//

#ifndef DEMONENGINE_EVENTBUS_HPP
#define DEMONENGINE_EVENTBUS_HPP

#include "core/base/interface/Interface.hpp"
#include "core/event/interface/IEventBus.hpp"

namespace event::base
{
    class EventBus : implements IEventBus
    {
    public:
        void callBusEvents()
        {
            callEvents();
        }
    };
}

#endif //DEMONENGINE_EVENTBUS_HPP
