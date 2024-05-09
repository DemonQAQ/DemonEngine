//
// Created by Demon on 2024/5/6.
//

#ifndef DEMONENGINE_ISCRIPTENTITY_HPP
#define DEMONENGINE_ISCRIPTENTITY_HPP

#include <string>
#include <utility>
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"

namespace script
{
    interface IScriptEntity : implements base::Object, implements base::INameable
    {
    protected:
        std::string name;
    public:
        IScriptEntity(const std::shared_ptr<base::UUID> &existingUuid,std::string name) :
        Object(existingUuid), name(std::move(name))
        {

        };

        int priority = 1;

        virtual bool hasMethod(std::string &methodName) = 0;

        virtual bool runMethod(std::string &methodName) = 0;

        virtual void unload() = 0;

        virtual void init() = 0;

        virtual void cleanupResources() = 0;

        [[nodiscard]] int getPriority() const
        {
            return priority;
        };

        void setPriority(int newPriority_)
        {
            this->priority = newPriority_;
        };

        void setName(const std::string &name_) override
        {
            this->name = name_;
        }

        [[nodiscard]] std::string getName() const override
        {
            return name;
        }
    };
}
#endif //DEMONENGINE_ISCRIPTENTITY_HPP
