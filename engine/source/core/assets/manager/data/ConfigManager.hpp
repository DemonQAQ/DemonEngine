//
// Created by Demon on 2024/3/7.
//

#ifndef DEMONENGINE_CONFIGMANAGER_HPP
#define DEMONENGINE_CONFIGMANAGER_HPP

#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IDataManager.hpp"
#include "core/io/interface/IFile.hpp"

namespace assets
{
    class ConfigManager : implements IDataManager
    {
    private:
        static std::unordered_map<std::shared_ptr<base::UUID>, std::shared_ptr<io::IFile>> loadedConfig;
    public:
        /**
         *
         * @params[0] const std::shared_ptr<base::UUID> &existingUuid   文件的uuid
         * @params[1] std::string                                       文件路径
         * @parmas[2] io::FileType                                      文件类型
         * */
        bool loadData(const std::vector<std::any>& params, bool isAssets = false) override;

        void unloadData(const std::vector<std::any>& params, bool isAssets = false) override;

        [[nodiscard]] bool isDataLoaded(const std::vector<std::any>& params) const override;

        void updateData(const std::vector<std::any>& params) override;

        std::optional<std::shared_ptr<io::IFile>> getResourceByUuid(const std::shared_ptr<base::UUID>& uuid_ptr);

        void onStart() override;

        void onStop() override;
    };
}

#endif //DEMONENGINE_CONFIGMANAGER_HPP
