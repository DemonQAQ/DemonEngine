//
// Created by Demon on 2024/4/26.
//

#ifndef DEMONENGINE_SCRIPTENTITY_HPP
#define DEMONENGINE_SCRIPTENTITY_HPP

#include <mono/metadata/assembly.h>
#include "core/base/interface/Interface.hpp"
#include "core/base/common/Object.hpp"
#include "core/base/interface/INameable.hpp"

namespace script
{
    class ScriptEntity : implements base::Object, implements base::INameable
    {
    private:
        int priority = 1;
        std::string name;
        MonoAssembly *scriptAssembly;
        MonoImage *scriptImage;
        std::unordered_map<std::string, MonoMethod *> methodCache;
    public:
        ScriptEntity(const std::shared_ptr<base::UUID> &existingUuid, std::string name, MonoAssembly *script,
                     int priority_ = 1);

        //todo 为hasMethod和runMethod函数生成另一个版本
        bool hasMethod(std::string &methodName);

        bool runMethod(std::string &methodName);

        void unload();

        void init();

        void setName(const std::string &name_) override;

        [[nodiscard]] std::string getName() const override;

        [[nodiscard]] int getPriority() const;

        void setPriority(int newPriority_);

        bool operator<(const ScriptEntity &other) const;

        bool operator>(const ScriptEntity &other) const;

        bool operator==(const ScriptEntity &other) const;
    };
}

#endif //DEMONENGINE_SCRIPTENTITY_HPP
