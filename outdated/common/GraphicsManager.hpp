//
// Created by Demon on 2023/5/29.
//

#ifndef DEMON_ENGINE_GRAPHICS_MANAGER_HPP
#define DEMON_ENGINE_GRAPHICS_MANAGER_HPP

#include "../interface/RuntimeModule.hpp"

namespace runtime
{
    class GraphicsManager : implements RuntimeModule
    {
    public:
        ~GraphicsManager() override = default;
    };
}

#endif //DEMON_ENGINE_GRAPHICS_MANAGER_HPP
