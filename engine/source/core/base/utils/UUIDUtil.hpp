//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_UUIDUTIL_HPP
#define DEMONENGINE_UUIDUTIL_HPP

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "core/base/common/UUID.hpp"

using namespace base;

namespace utils
{
    static UUID getUUID()
    {
        return UUID();
    }

    static UUID getUUID(const std::string &str, bool isUUID)
    {
        if (isUUID)
        {
            return UUID(str); // 直接从字符串构造UUID
        } else
        {
            // 使用一个预定义的UUID作为命名空间
            boost::uuids::string_generator gen;
            boost::uuids::uuid dns_namespace_uuid = gen("6ba7b810-9dad-11d1-80b4-00c04fd430c8");
            boost::uuids::name_generator_sha1 name_gen(dns_namespace_uuid);

            return UUID(name_gen(str));
        }
    }
}
#endif //DEMONENGINE_UUIDUTIL_HPP
