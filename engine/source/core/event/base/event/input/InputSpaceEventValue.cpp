//
// Created by Demon on 2024/4/24.
//
#include "KeyDownEvent.hpp"
#include "KeyUpEvent.hpp"
#include "MouseMoveEvent.hpp"
#include "ScrollEvent.hpp"

using namespace event::base;

const std::string KeyDownEvent::eventName = "event_base_KeyDownEvent";

const std::string KeyUpEvent::eventName = "event_base_KeyUpEvent";

const std::string MouseMoveEvent::eventName = "event_base_MouseMoveEvent";

const std::string ScrollEvent::eventName = "event_base_ScrollEvent";