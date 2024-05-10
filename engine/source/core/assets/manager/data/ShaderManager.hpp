//
// Created by Demon on 2023/12/12.
//

#ifndef DEMONENGINE_SHADERMANAGER_HPP
#define DEMONENGINE_SHADERMANAGER_HPP

#include <map>
#include "core/base/interface/Interface.hpp"
#include "core/assets/interface/IDataManager.hpp"
#include "core/base/render/Shader.hpp"

namespace assets
{
    class ShaderManager : implements IDataManager
    {
    private:
        static std::map<std::shared_ptr<base::UUID>, std::shared_ptr<base::Shader>> loadedShaders;
    public:
        ShaderManager();

        bool loadData(const std::vector<std::any> &params, bool isAssets = false) override;

        void unloadData(const std::vector<std::any> &params, bool isAssets = false) override;

        [[nodiscard]] bool isDataLoaded(const std::vector<std::any>& params) const override;

        void updateData(const std::vector<std::any>& params) override;

        std::optional<std::shared_ptr<base::Shader>> getResourceByUuid(const std::shared_ptr<base::UUID>& uuid_ptr);

        void onStart() override;

        void onStop() override;
    };
}

#endif //DEMONENGINE_SHADERMANAGER_HPP
