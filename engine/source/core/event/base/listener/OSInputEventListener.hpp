//
// Created by Demon on 2024/4/23.
//

#ifndef DEMONENGINE_OSINPUTEVENTLISTENER_HPP
#define DEMONENGINE_OSINPUTEVENTLISTENER_HPP

#include <iostream>
#include <runtime/base/RuntimeApplication.hpp>
#include "core/base/interface/Interface.hpp"
#include "core/event/interface/IEventListener.hpp"
#include "core/event/base/event/input/KeyDownEvent.hpp"
#include "core/event/base/event/input/KeyUpEvent.hpp"
#include "core/event/base/event/input/MouseMoveEvent.hpp"
#include "core/event/base/event/input/ScrollEvent.hpp"

namespace event::base
{
    class OSInputEventListener : implements IEventListener
    {
    public:
        OSInputEventListener()
        {
            registerEvent(KeyDownEvent::staticGetEventName(), &OSInputEventListener::onMoveKeyDown);
            registerEvent(KeyUpEvent::staticGetEventName(), &OSInputEventListener::onMoveKeyUp);
            registerEvent(MouseMoveEvent::staticGetEventName(), &OSInputEventListener::onMouseMove);
            registerEvent(ScrollEvent::staticGetEventName(), &OSInputEventListener::onMouseScroll);
        }

        void onMoveKeyDown(std::shared_ptr<KeyDownEvent> &e)
        {
            std::cout << "KeyDown: " << e->getKeyType() << std::endl;

            auto scene = runtimeApp.getScene();
            auto mainCameraEntity = scene->getMainCameraEntity();

            CameraMovement direction = getCameraMovement(e->getKeyType());
            if (direction == CameraMovement::NONE)return;

            mainCameraEntity->processKeyboard(direction, true);
            mainCameraEntity->setCameraMovementState(true);
        }

        void onMoveKeyUp(std::shared_ptr<KeyUpEvent> &e)
        {
            std::cout << "KeyUp: " << e->getKeyType() << std::endl;

            auto scene = runtimeApp.getScene();
            auto mainCameraEntity = scene->getMainCameraEntity();

            CameraMovement direction = getCameraMovement(e->getKeyType());
            if (direction == CameraMovement::NONE)return;

            //todo 让相机进入update
            mainCameraEntity->processKeyboard(direction, false);
            mainCameraEntity->setCameraMovementState(false);
        }

        void onMouseMove(std::shared_ptr<MouseMoveEvent> &e)
        {
            std::cout << "onMouseMove" << std::endl;
        }

        void onMouseScroll(std::shared_ptr<ScrollEvent> &e)
        {
            std::cout << "onMouseScroll" << std::endl;
        }

    private:
        CameraMovement getCameraMovement(KeyType type)
        {
            switch (type)
            {
                case KeyType::KEY_W:
                    return CameraMovement::FORWARD;
                case KeyType::KEY_S:
                    return CameraMovement::BACKWARD;
                case KeyType::KEY_A:
                    return CameraMovement::LEFT;
                case KeyType::KEY_D:
                    return CameraMovement::RIGHT;
                default:
                    return NONE;
            }
        }
    };
}


#endif //DEMONENGINE_OSINPUTEVENTLISTENER_HPP
