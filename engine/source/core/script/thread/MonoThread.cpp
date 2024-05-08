//
// Created by Demon on 2024/5/1.
//

#include <mono/metadata/appdomain.h>
#include <core/base/lib/BoolDefine.hpp>
#include <mono/metadata/threads.h>
#include <mono/jit/jit.h>
#include "MonoThread.hpp"

using namespace script;

script::MonoThread::MonoThread(const std::string &domainName_, const std::shared_ptr<io::XmlConfiguration> &config_)
        : domainName(domainName_), config(config_)
{
    initializeDomain({});
}

script::MonoThread::~MonoThread()
{
    if (thread && thread->joinable())
    {
        thread->join();
    }
    if (domain)
    {
        mono_domain_unload(domain);
        mono_jit_cleanup(domain);
        domain = nullptr;
    }
}

void script::MonoThread::start()
{
    if (!thread || !thread->joinable())
    {
        thread = std::make_unique<boost::thread>(&MonoThread::threadFunction, this);
    }
}

void script::MonoThread::stop()
{
    if (thread && thread->joinable())
    {
        thread->interrupt();
        thread->join();
    }
}

bool script::MonoThread::isRunning() const
{
    return thread && thread->joinable();
}


void script::MonoThread::threadFunction()
{
    mono_thread_attach(domain);
    try
    {
        while (isRunning())
        {
            std::function < void() > task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                cv.wait(lock, [this]
                {
                    return !tasks.empty() || !isRunning();
                });
                if (!isRunning() && tasks.empty())break;
                task = tasks.front();
                tasks.pop();
            }
            if (task)task();
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