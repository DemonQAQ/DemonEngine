//
// Created by Demon on 2024/4/23.
//

#ifndef DEMONENGINE_IEVENTBUS_HPP
#define DEMONENGINE_IEVENTBUS_HPP

#include <unordered_map>
#include <deque>
#include "core/base/interface/Interface.hpp"
#include "IEventListener.hpp"

namespace event
{
    interface IEventBus
    {
    private:
        std::unordered_map<std::string, std::vector<std::shared_ptr<IEventListener>>> listeners;
        std::deque<std::shared_ptr<IEvent>> currentFrameEvents;
        std::deque<std::shared_ptr<IEvent>> nextFrameEvents;
    protected:
        void callEvents()
        {
            currentFrameEvents.swap(nextFrameEvents);
            nextFrameEvents.clear();

            // 处理所有当前帧的事件
            while (!currentFrameEvents.empty())
            {
                auto event = currentFrameEvents.front();
                currentFrameEvents.pop_front();
                auto it = listeners.find(event->getEventName());
                if (it != listeners.end())
                {
                    for (auto &listener: it->second)
                    {
                        listener->receiveEvent(event);
                    }
                }
            }
        }

    public:
        void subscribe(const std::shared_ptr<IEventListener> &listener)
        {
            //todo 注册监听器
        }

        void callEvent(const std::shared_ptr<IEvent> &event)
        {
            nextFrameEvents.push_back(event);
        }
    };
}

#endif //DEMONENGINE_IEVENTBUS_HPP
