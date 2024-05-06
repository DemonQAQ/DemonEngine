//
// Created by Demon on 2024/5/1.
//

#include <mono/metadata/appdomain.h>
#include <core/base/lib/BoolDefine.hpp>
#include <mono/metadata/threads.h>
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
    while (isRunning())
    {
        std::function < void() > task;
        {
            std::cout << "start exe" << std::endl;
            std::unique_lock<std::mutex> lock(queueMutex);
            std::cout << "try to get lock" << std::endl;
            cv.wait(lock, [&]
            { return !tasks.empty(); });
            std::cout << "try to get task" << std::endl;
            task = tasks.front();
            tasks.pop();
            std::cout << "end exe" << std::endl;
        }
        task();
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