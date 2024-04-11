//
// Created by Demon on 2024/4/9.
//

#ifndef DEMONENGINE_METADATA_HPP
#define DEMONENGINE_METADATA_HPP

#include <any>
#include <string>
#include <unordered_map>
#include "core/base/interface/Interface.hpp"

namespace base
{
    class Metadata
    {
    private:
        std::unordered_map<std::string, std::any> data;
    public:
        virtual ~Metadata() = default;

        [[nodiscard]] std::any getValue(const std::string &key) const
        {
            auto it = data.find(key);
            if (it != data.end())
            {
                return it->second;
            }
            return {};
        }

        void setValue(const std::string &key, const std::any &value)
        {
            data[key] = value;
        }

        [[nodiscard]] bool containsKey(const std::string &key) const
        {
            return data.find(key) != data.end();
        }

        [[nodiscard]] std::vector<std::string> getKeys() const
        {
            std::vector<std::string> keys;
            for (const auto &pair: data)
            {
                keys.push_back(pair.first);
            }
            return keys;
        }
    };
}

#endif //DEMONENGINE_METADATA_HPP
