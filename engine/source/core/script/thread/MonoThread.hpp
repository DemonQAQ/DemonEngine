//
// Created by Demon on 2024/5/1.
//

#ifndef DEMONENGINE_MONOTHREAD_HPP
#define DEMONENGINE_MONOTHREAD_HPP

#include <memory>
#include "core/base/interface/Interface.hpp"
#include "core/script/interface/IScriptRuntimeThread.hpp"
#include "mono/utils/mono-forward.h"
#include <boost/thread.hpp>
#include <core/io/config/XmlConfiguration.hpp>
#include <queue>

namespace script
{
    class MonoThread : implements IScriptRuntimeThread
    {
    private:
        std::shared_ptr<boost::thread> thread;
        std::string domainName;
        std::shared_ptr<io::XmlConfiguration> config;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::condition_variable cv;
    protected:
        MonoDomain *domain = nullptr;
    public:
        explicit MonoThread(const std::string &domainName_, const std::shared_ptr<io::XmlConfiguration> &config_ = nullptr);

        ~MonoThread() override;

        void start() override;

        void stop() override;

        [[nodiscard]] bool isRunning() const override;

        void initializeDomain(const std::vector<std::any> &params) override;

        void destroyDomain() override;

        void threadFunction();

        void submitTask(std::function<void()> task) override;
    };
}

#endif //DEMONENGINE_MONOTHREAD_HPP
