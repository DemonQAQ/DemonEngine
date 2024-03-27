//
// Created by Demon on 2023/12/20.
//

#ifndef DEMONENGINE_OBJECT_HPP
#define DEMONENGINE_OBJECT_HPP

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "core/base/interface/Interface.hpp"
#include "UUID.hpp"

namespace base
{
    class Object
    {
    public:
        Object() : uuid(UUID()) {}

        explicit Object(const std::string& str) : uuid(str.empty() ? UUID() : UUID(str)) {}

        explicit Object(const UUID& existingUuid) : uuid(existingUuid) {}

        virtual ~Object() = default;

        [[nodiscard]] std::string getUuidStr() const
        {
            return uuid.toString();
        }

        [[nodiscard]] UUID getUUID() const
        {
            return uuid;
        }

    private:
        UUID uuid;
    };
}

#endif //DEMONENGINE_OBJECT_HPP
