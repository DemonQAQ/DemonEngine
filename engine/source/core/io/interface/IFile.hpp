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
    protected:
        std::string path;
    public:
        IFile(const std::shared_ptr<base::UUID> &existingUuid, const std::string &path_) :
        Object(existingUuid),path(path_){}

        virtual void load() = 0;

        virtual void save() const = 0;

        virtual bool isEmpty() const = 0;

        virtual std::shared_ptr<char[]> toCharList() const = 0;
    };
}
#endif //DEMONENGINE_IFILE_HPP
