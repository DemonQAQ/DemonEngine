//
// Created by Demon on 2023/5/19.
//
/**
 * @file    RuntimeModule.h
 * @brief   none
 * @author  Demon
 * @date    2023/5/19
 */
#ifndef DEMON_ENGINE_RUNTIME_MODULE_H
#define DEMON_ENGINE_RUNTIME_MODULE_H

#include "Interface.h"

namespace runtime
{
    Interface RuntimeModule
    {
    public:
        virtual ~RuntimeModule() = default;

        virtual int initialize() = 0;

        virtual void finalize() = 0;

        virtual void tick() = 0;
    };
}

#endif
