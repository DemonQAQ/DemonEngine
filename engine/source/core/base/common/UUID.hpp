//
// Created by Demon on 2023/12/19.
//

#ifndef DEMONENGINE_UUID_HPP
#define DEMONENGINE_UUID_HPP

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include <string>

namespace base
{
    class UUID
    {
    public:
        // 默认构造函数，生成一个随机UUID
        UUID() : uuid(boost::uuids::random_generator()())
        {}

        // 从boost::uuids::uuid构造
        explicit UUID(const boost::uuids::uuid &u) : uuid(u)
        {}

        // 从字符串构造
        UUID(const std::string &str) : uuid(boost::uuids::string_generator{}(str))
        {}

        [[nodiscard]] std::string toString() const
        {
            return boost::uuids::to_string(uuid);
        }

        bool operator==(const UUID &other) const
        {
            return uuid == other.uuid;
        }

        bool operator!=(const UUID &other) const
        {
            return uuid != other.uuid;
        }

        bool operator<(const UUID &other) const
        {
            return uuid < other.uuid;
        }

        bool operator>(const UUID &other) const
        {
            return uuid > other.uuid;
        }

        bool operator<=(const UUID &other) const
        {
            return uuid <= other.uuid;
        }

        bool operator>=(const UUID &other) const
        {
            return uuid >= other.uuid;
        }

    private:
        boost::uuids::uuid uuid;
    };
}
#endif //DEMONENGINE_UUID_HPP