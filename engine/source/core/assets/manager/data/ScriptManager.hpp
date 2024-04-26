//
// Created by Demon on 2024/4/26.
//

#ifndef DEMONENGINE_SCRIPTMANAGER_HPP
#define DEMONENGINE_SCRIPTMANAGER_HPP

#include <mono/metadata/image.h>
#include <mono/utils/mono-forward.h>
#include <core/script/ScriptEntity.hpp>
#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IDataManager.hpp"

namespace assets
{
    class ScriptManager : implements IDataManager
    {
    private:
        MonoDomain* scriptDomain;
        static std::unordered_map<std::shared_ptr<base::UUID>,std::shared_ptr<script::ScriptEntity>> loadScripts;
    public:
        ScriptManager();
        ~ScriptManager() override;

        /**
         * @params[0] std::shared_ptr<base::UUID> &existingUuid         脚本实体uuid
         * @params[1] std::string path                                  脚本路径
         * */
        bool loadData(const std::vector<std::any> &params) override;

        void unloadData(const std::vector<std::any> &params) override;

        [[nodiscard]] bool isDataLoaded(const std::vector<std::any> &params) const override;

        void updateData(const std::vector<std::any> &params) override;

        std::optional<std::shared_ptr<script::ScriptEntity>> getResourceByUuid(const std::shared_ptr<base::UUID> &uuid_ptr);
    };
}

#endif //DEMONENGINE_SCRIPTMANAGER_HPP
