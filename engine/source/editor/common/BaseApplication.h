//
// Created by Demon on 2023/5/19.
//
/**
 * @file    BaseApplication.h
 * @brief   none
 * @author  Demon
 * @date    2023/5/19
 */
#ifndef DEMONENGINE_BASEAPPLICATION_H
#define DEMONENGINE_BASEAPPLICATION_H

#include "../interface/Application.h"

namespace runtime
{
    Interface BaseApplication : implements Application
    {
    public:
        ~BaseApplication() override = default;

        int initialize() override;

        void finalize() override;

        void tick() override;

        bool isQuit() override;

    protected:
        bool quit;
    };
}

#endif //DEMONENGINE_BASEAPPLICATION_H
