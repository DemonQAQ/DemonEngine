//
// Created by Demon on 2023/5/19.
//
/**
 * @file    OpenglApplication.h
 * @brief   none
 * @author  Demon
 * @date    2023/5/19
 */
#ifndef DEMONENGINE_BASEAPPLICATION_HPP
#define DEMONENGINE_BASEAPPLICATION_HPP

#include "../interface/Application.hpp"

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

#endif //DEMONENGINE_BASEAPPLICATION_HPP
