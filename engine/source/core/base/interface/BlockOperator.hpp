//
// Created by Demon on 2024/4/9.
//

#ifndef DEMONENGINE_BLOCKOPERATOR_HPP
#define DEMONENGINE_BLOCKOPERATOR_HPP

#include <memory>
#include <core/base/common/UUID.hpp>
#include "Interface.hpp"
#include "core/base/common/Metadata.hpp"
#include "core/io/config/YamlConfiguration.hpp"

namespace base
{

    class BlockOperator
    {
    public:
        BlockOperator() = default;

        ~BlockOperator() = default;

        // 子类需要实现此方法，负责将metadata中的负责管理的数据写入指定yaml
        virtual void writeToBlock(std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml) = 0;

        // 子类需要实现此方法，负责从指定yaml中读取数据存入metadata
        virtual void readFromBlock(std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml) = 0;

        // 子类第一次创建时调用此方法初始化metadata
        virtual void initBlock(std::shared_ptr<Metadata> &metadata, const std::vector<std::any> &params) = 0;
    };

}

#endif //DEMONENGINE_BLOCKOPERATOR_HPP
