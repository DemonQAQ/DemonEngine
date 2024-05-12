//
// Created by Demon on 2024/3/29.
//

#ifndef DEMONENGINE_ISERIALIZABLE_HPP
#define DEMONENGINE_ISERIALIZABLE_HPP

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>
#include "core/base/interface/Interface.hpp"


namespace io
{
    class ISerializable
    {
    public:
        virtual ~ISerializable() = default;

        [[nodiscard]] virtual std::string serialize() const = 0;

        virtual void deserialize(const std::string &data) = 0;
    };
}

#endif //DEMONENGINE_ISERIALIZABLE_HPP
