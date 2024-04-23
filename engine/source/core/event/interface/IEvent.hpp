//
// Created by Demon on 2024/4/23.
//

#ifndef DEMONENGINE_IEVENT_HPP
#define DEMONENGINE_IEVENT_HPP

#include <string>
#include <chrono>
#include "core/base/interface/Interface.hpp"

namespace event
{
    interface IEvent
    {
    private:
        bool isCancel_ = false;
        long long timestamp;
    public:
        IEvent() : timestamp(std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count())
        {

        }

        IEvent(long long timestamp_) : timestamp(timestamp_)
        {

        }

        [[nodiscard]] virtual std::string getEventName() const = 0;

        [[nodiscard]] bool isCancel() const
        {
            return isCancel_;
        }

        void setCancel(bool value)
        {
            isCancel_ = value;
        }

        [[nodiscard]] long long getTimestamp() const
        {
            return timestamp;
        }
    };
}
#endif //DEMONENGINE_IEVENT_HPP
