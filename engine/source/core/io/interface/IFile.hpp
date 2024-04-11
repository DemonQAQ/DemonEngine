//
// Created by Demon on 2024/3/11.
//

#ifndef DEMONENGINE_IFILE_HPP
#define DEMONENGINE_IFILE_HPP

#include <string>
#include <optional>
#include <any>
#include "core/base/common/Object.hpp"

namespace io
{
    class IFile : implements base::Object
    {
    public:
        virtual void load(const std::string &path) = 0;

        virtual void save(const std::string &path) const = 0;

        explicit IFile(const std::string &path) : Object(path, false)
        {}

    };
}
#endif //DEMONENGINE_IFILE_HPP
