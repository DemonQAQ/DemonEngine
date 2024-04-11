//
// Created by Demon on 2023/12/19.
//

#ifndef DEMONENGINE_UUID_HPP
#define DEMONENGINE_UUID_HPP

#include "boost/uuid/uuid.hpp"
#include "boost/uuid/uuid_generators.hpp"
#include "boost/uuid/uuid_io.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/functional/hash.hpp>
#include <string>
#include "iostream"

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

        // 从字符串构造UUID
        explicit UUID(const std::string &str, bool isUUID)
        {
            if (isUUID)
            {
                boost::uuids::string_generator gen;
                try
                {
                    uuid = gen(str);
                } catch (const std::exception &e)
                {
                    std::cerr << "Exception creating UUID from string: " << e.what() << std::endl;
                    throw;
                }
            } else
            {
                static const boost::uuids::uuid name_space_uuid = boost::uuids::string_generator()(
                        "6ba7b810-9dad-11d1-80b4-00c04fd430c8");
                boost::uuids::name_generator_sha1 gen(name_space_uuid);
                uuid = gen(str.c_str(), str.size());
            }
        }

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

        [[nodiscard]] const boost::uuids::uuid &getBoostUuid() const
        {
            return uuid;
        }

    private:
        boost::uuids::uuid uuid;
    };
}

namespace std
{
    template<>
    struct hash<base::UUID>
    {
        size_t operator()(const base::UUID &uuid) const noexcept
        {
            const auto &boost_uuid = uuid.getBoostUuid();
            return boost::hash<boost::uuids::uuid>()(boost_uuid);
        }
    };
}

struct UUIDHash
{
    size_t operator()(const std::shared_ptr<base::UUID> &uuid_ptr) const noexcept
    {
        if (!uuid_ptr) return 0; // 对空指针返回0
        std::hash<base::UUID> hasher;
        return hasher(*uuid_ptr);
    }
};

struct UUIDEqual
{
    bool operator()(const std::shared_ptr<base::UUID> &lhs, const std::shared_ptr<base::UUID> &rhs) const
    {
        if (!lhs || !rhs) return !lhs && !rhs; // 两个空指针视为相等
        return *lhs == *rhs;
    }
};

#endif //DEMONENGINE_UUID_HPP