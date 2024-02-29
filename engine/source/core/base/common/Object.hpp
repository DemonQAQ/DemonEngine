//
// Created by Demon on 2023/12/20.
//

#ifndef DEMONENGINE_OBJECT_HPP
#define DEMONENGINE_OBJECT_HPP

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "core/base/interface/Interface.hpp"

namespace base
{
    class Object
    {
    public:
        // 默认构造函数
        Object() : uuid(boost::uuids::random_generator()())
        {}

        // 获取对象的UUID
        [[nodiscard]] std::string getUuid() const
        {
            return boost::uuids::to_string(uuid);
        }

        // 虚析构函数，确保子类的正确析构
        virtual ~Object() = default;

    private:
        // 对象的唯一标识符
        const boost::uuids::uuid uuid;
    };
}

#endif //DEMONENGINE_OBJECT_HPP
