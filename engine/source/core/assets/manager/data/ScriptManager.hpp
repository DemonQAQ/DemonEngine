//
// Created by Demon on 2024/4/26.
//

#ifndef DEMONENGINE_SCRIPTMANAGER_HPP
#define DEMONENGINE_SCRIPTMANAGER_HPP

#include <mono/utils/mono-forward.h>
#include <mono/metadata/image.h>
#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IDataManager.hpp"

namespace assets
{
    class ScriptManager : implements IDataManager
    {
    private:
        MonoDomain* scriptDomain;
        std::vector<MonoAssembly*> scriptAssemblies;
    public:
        ScriptManager();
        ~ScriptManager() override;
        /**
         * 传入一个参数，类型为std::string，含义为目标脚本的路径，尝试解析
         * */
        bool loadData(const std::vector<std::any> &params) override;

        void unloadData(const std::vector<std::any> &params) override;

        [[nodiscard]] bool isDataLoaded(const std::vector<std::any> &params) const override;

        void updateData(const std::vector<std::any> &params) override;
    };
}

#endif //DEMONENGINE_SCRIPTMANAGER_HPP
