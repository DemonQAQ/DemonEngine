﻿//
// Created by Demon on 2024/4/23.
//

#ifndef DEMONENGINE_IEVENTLISTENER_HPP
#define DEMONENGINE_IEVENTLISTENER_HPP

#include <memory>
#include <unordered_map>
#include <functional>
#include <set>
#include "core/base/interface/Interface.hpp"
#include "IEvent.hpp"

namespace event
{
    interface IEventListener
    {
    protected:
        struct CallbackInfo
        {
            std::function<void(std::shared_ptr<IEvent>)> callback;
            long long id;

            bool operator<(const CallbackInfo &other) const
            {
                return id < other.id;
            }
        };

        std::unordered_map<std::string, std::set<CallbackInfo>> eventCallbacks;
        long long nextId = 0;

    public:
        virtual void
        registerEventCallback(const std::string &eventName, std::function<void(std::shared_ptr<IEvent>)> callback) final
        {
            eventCallbacks[eventName].insert({callback, nextId++});
        }

        virtual void unregisterEventCallback(const std::string &eventName, long long id) final
        {
            auto it = eventCallbacks.find(eventName);
            if (it != eventCallbacks.end())
            {
                auto &set = it->second;
                for (auto iter = set.begin(); iter != set.end(); ++iter)
                {
                    if (iter->id == id)
                    {
                        set.erase(iter);
                        break;
                    }
                }
            }
        }

        virtual int receiveEvent(std::shared_ptr<IEvent> &event) final
        {
            std::string eventName = event->getEventName();
            auto it = eventCallbacks.find(eventName);
            if (it != eventCallbacks.end())
            {
                for (auto &info: it->second)
                {
                    info.callback(event);
                }
                return 0;
            }
            return -1;
        }
    };
}

#endif //DEMONENGINE_IEVENTLISTENER_HPP
