//
// Created by Demon on 2024/4/26.
//

#ifndef DEMONENGINE_ASSEMBLYSCRIPTENTITY_HPP
#define DEMONENGINE_ASSEMBLYSCRIPTENTITY_HPP

#include <mono/metadata/assembly.h>
#include <core/script/interface/IScriptEntity.hpp>
#include "core/base/interface/Interface.hpp"

namespace script
{
    class AssemblyScriptEntity : implements IScriptEntity
    {
    private:
        int priority = 1;
        MonoAssembly *scriptAssembly;
        MonoImage *scriptImage;
        std::unordered_map<std::string, MonoMethod *> methodCache;
    public:
        AssemblyScriptEntity(const std::shared_ptr<base::UUID> &existingUuid, const std::string &name,
                             MonoAssembly *script, MonoImage *scriptImage, int priority_ = 1);

        //todo 为hasMethod和runMethod函数生成另一个版本
        bool hasMethod(std::string &methodName) override;

        bool runMethod(std::string &methodName) override;

        void unload() override;

        void init() override;

        bool operator<(const AssemblyScriptEntity &other) const;

        bool operator>(const AssemblyScriptEntity &other) const;

        bool operator==(const AssemblyScriptEntity &other) const;
    };
}

#endif //DEMONENGINE_ASSEMBLYSCRIPTENTITY_HPP
