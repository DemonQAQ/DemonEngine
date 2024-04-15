//
// Created by Demon on 2023/12/15.
//

#ifndef DEMONENGINE_UUIDUTIL_HPP
#define DEMONENGINE_UUIDUTIL_HPP

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "core/base/common/UUID.hpp"
#include <chrono>

namespace utils::uuidUtil
{
    /**
     * 处理获得获取唯一id的字符串
     *
     * @param str 字符串
     * @return 用于构造唯一uuid的字符串
     * */
    static std::string getUUID(const std::string &str)
    {
        boost::uuids::uuid random_uuid = boost::uuids::random_generator()();

        auto now = std::chrono::system_clock::now();
        auto duration = now.time_since_epoch();
        auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        std::stringstream ss;
        ss << milliseconds << "-" << random_uuid << "-" << str;

        return ss.str();
    }

    /**
     * 根据固定规则生成可复现的uuid字符串，使用同一输入一定会得到同一输出
     *
     * @param str 字符串
     * @return 根据固定规则生成的字符串
     * */
    static std::string getReappearUUID(const std::string &str)
    {
        boost::uuids::uuid dns_namespace_uuid = boost::uuids::string_generator()(
                "6ba7b810-9dad-11d1-80b4-00c04fd430c8");
        boost::uuids::name_generator gen(dns_namespace_uuid);
        boost::uuids::uuid uuid = gen(str);
        std::string uuid_str = boost::uuids::to_string(uuid);
        return uuid_str;
    }

    static bool isUUIDStr(const std::string &str)
    {
        try
        {
            boost::uuids::string_generator gen;
            boost::uuids::uuid uuid = gen(str);
            return true;
        } catch (...)
        {
            return false;
        }
    }

}
#endif //DEMONENGINE_UUIDUTIL_HPP
