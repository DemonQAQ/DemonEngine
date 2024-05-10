//
// Created by Demon on 2024/5/1.
//

#include <mono/metadata/appdomain.h>
#include <core/base/lib/BoolDefine.hpp>
#include <mono/metadata/threads.h>
#include <mono/jit/jit.h>
#include "MonoThread.hpp"

using namespace script;

script::MonoThread::MonoThread(const std::string &domainName_, const std::string &threadName,
                               const std::shared_ptr<io::XmlConfiguration> &config_)
        : domainName(domainName_), config(config_), name(threadName)
{
    initializeDomain({});
}

script::MonoThread::~MonoThread()
{
    threadRunning = false;
    if (thread && thread->joinable())
    {
        thread->join();
    }

    thread.reset();

    if (domain)
    {
        mono_domain_unload(domain);
        domain = nullptr;
    }
}

void script::MonoThread::start()
{
    std::cout << "MonoThread start" << threadRunning << std::endl;
    if (!thread || !thread->joinable())
    {
        threadRunning = true;
        thread = std::make_unique<boost::thread>(&MonoThread::threadFunction, this);
    }
}

void script::MonoThread::stop()
{
    if (threadRunning)
    {
        threadRunning = false;
        cv.notify_all();
        if (thread && thread->joinable())
        {
            thread->interrupt();
            thread->join();
        }
    }
    std::cout << "threadRunning:" << threadRunning << std::endl;
}

bool script::MonoThread::isRunning() const
{
    return thread && thread->joinable() && threadRunning;
}


void script::MonoThread::threadFunction()
{
    mono_thread_attach(domain);
    try
    {
        while (isRunning())
        {
            std::cout << name << " threadRunning:" << threadRunning << std::endl;
            std::function < void() > task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                if (!cv.wait_for(lock, std::chrono::seconds(1), [this]
                { return !tasks.empty() || !isRunning(); }))
                {
                    std::cout << name << " Wait timeout or condition met." << std::endl;
                }

                if (!isRunning())
                {
                    std::cout << name << "Exiting loop as isRunning() returned false." << std::endl;
                    break;
                }

                if (!tasks.empty())
                {
                    task = tasks.front();
                    tasks.pop();
                }
            }
            if (task)
            {
                std::cout << "Executing task." << std::endl;
                task();
            }
        }
    } catch (const boost::thread_interrupted &)
    {
        std::cout << "Thread was interrupted" << std::endl;
    }

    mono_thread_detach(mono_thread_current());
}


void script::MonoThread::initializeDomain(const std::vector<std::any> &params)
{
    const char *domainNameCStr = domainName.c_str();
    char *nonConstDomainNameCStr = const_cast<char *>(domainNameCStr);
    char *configCStr = config ? const_cast<char *>(config->toCharList().get()) : NULL;

    if (!domain)
    {
        domain = mono_domain_create_appdomain(nonConstDomainNameCStr, configCStr);
        mono_domain_set(domain, TRUE);
    }
}

void script::MonoThread::destroyDomain()
{
    if (domain)
    {
        mono_domain_unload(domain);
        domain = nullptr;
    }
}

void script::MonoThread::submitTask(std::function<void()> task)
{
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        tasks.push(std::move(task));
    }
    cv.notify_one();
}

void script::MonoThread::setName(const std::string &name_)
{
    name = name_;
}

std::string script::MonoThread::getName() const
{
    return name;
}