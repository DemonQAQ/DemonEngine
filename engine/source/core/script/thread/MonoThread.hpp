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
#include <core/base/interface/INameable.hpp>

namespace script
{
    class MonoThread : implements IScriptRuntimeThread ,implements base::INameable
    {
    private:
        std::shared_ptr<boost::thread> thread;
        std::string domainName;
        std::shared_ptr<io::XmlConfiguration> config;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::mutex startStopMutex;
        std::condition_variable cv;
        std::atomic<bool> threadRunning;
        std::string name;
    protected:
        MonoDomain *domain = nullptr;
    public:
        explicit MonoThread(const std::string &domainName_, const std::string &threadName, const std::shared_ptr<io::XmlConfiguration> &config_ = nullptr);

        ~MonoThread() override;

        void start() override;

        void stop() override;

        [[nodiscard]] bool isRunning() const override;

        void initializeDomain(const std::vector<std::any> &params) override;

        void destroyDomain() override;

        void threadFunction();

        void submitTask(std::function<void()> task) override;

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;
    };
}

#endif //DEMONENGINE_MONOTHREAD_HPP
