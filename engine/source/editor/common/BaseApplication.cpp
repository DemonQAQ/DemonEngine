//
// Created by Demon on 2023/5/19.
//
/**
 * @file    BaseApplication.cpp
 * @brief   none
 * @author  Demon
 * @date    2023/5/19
 */

#include "BaseApplication.hpp"
int runtime::BaseApplication::initialize()
{
    quit = false;
    return 0;
}

void runtime::BaseApplication::finalize()
{

}

void runtime::BaseApplication::tick()
{

}

bool runtime::BaseApplication::isQuit()
{
    return quit;
}