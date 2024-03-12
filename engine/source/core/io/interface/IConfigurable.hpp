//
// Created by Demon on 2024/3/11.
//

#ifndef DEMONENGINE_ICONFIGURABLE_HPP
#define DEMONENGINE_ICONFIGURABLE_HPP

#include <string>
#include <optional>
#include <any>
#include "../../base/interface/Interface.hpp"
#include "../../base/common/Object.hpp"

namespace io
{
    class IConfigurable : implements Object
    {
    public:
        virtual void load(const std::string &path) = 0;

        virtual void save(const std::string &path) const = 0;

        explicit IConfigurable(const std::string &path) : Object(path){}

    };
}
#endif //DEMONENGINE_ICONFIGURABLE_HPP
