//
// Created by Demon on 2024/5/1.
//

#include "MonoScriptThread.hpp"

using namespace script;

MonoScriptThread::MonoScriptThread()
{
    // Constructor body
}

MonoScriptThread::~MonoScriptThread()
{
    // Ensure the thread is stopped before destruction
    if (thread && thread->joinable())
    {
        thread->join();
    }
}

void MonoScriptThread::start()
{
    if (!thread || !thread->joinable())
    {
        //thread = std::make_unique<boost::thread>(&MonoScriptThread::threadFunction, this);
    }
}

void MonoScriptThread::stop()
{
    if (thread && thread->joinable())
    {
        // Signal thread to stop, e.g., using a condition or an atomic flag
        thread->interrupt(); // Requires handling in threadFunction
        thread->join();
    }
}

bool MonoScriptThread::isRunning() const
{
    return thread && thread->joinable();
}

//void MonoScriptThread::threadFunction()
//{
//    // Thread's main function goes here
//}

void MonoScriptThread::initializeDomain(const std::vector<std::any> &params)
{
    // Initialize Mono domain
}

void MonoScriptThread::destroyDomain()
{
    // Cleanup Mono domain
}
