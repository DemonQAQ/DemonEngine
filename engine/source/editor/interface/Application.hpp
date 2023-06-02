//
// Created by Demon on 2023/5/19.
//
/**
 * @file    Application.h
 * @brief   none
 * @author  Demon
 * @date    2023/5/19
 */
#ifndef DEMON_ENGINE_APPLICATION_H
#define DEMON_ENGINE_APPLICATION_H

#include "Interface.hpp"
#include "RuntimeModule.hpp"

namespace runtime
{
    Interface Application : implements RuntimeModule
    {
    public:
        virtual ~Application() = default;

        virtual int initialize() = 0;

        virtual void finalize() = 0;

        virtual void tick() = 0;

        virtual bool isQuit() = 0;
    };
}

#endif //DEMON_ENGINE_APPLICATION_H
