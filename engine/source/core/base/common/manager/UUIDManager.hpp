//
// Created by Demon on 2024/4/11.
//

#ifndef DEMONENGINE_UUIDMANAGER_HPP
#define DEMONENGINE_UUIDMANAGER_HPP

#include <memory>
#include <vector>
#include <string>
#include "core/base/common/UUID.hpp"
#include "core/base/utils/UUIDUtil.hpp"
#include <unordered_set>

namespace base
{
    class UUIDManager
    {
    private:
        static std::unordered_set<std::shared_ptr<base::UUID>, UUIDHash, UUIDEqual, std::allocator<std::shared_ptr<base::UUID>>> uuids;
        static std::unordered_set<std::shared_ptr<base::UUID>, UUIDHash, UUIDEqual, std::allocator<std::shared_ptr<base::UUID>>> reappearUuids;
    public:
        static std::shared_ptr<UUID> getUUID(const std::string &str, bool canReappear = false)
        {
            if (!utils::uuidUtil::isUUIDStr(str))
            {
                throw std::invalid_argument("String does not conform to UUID format");
            }

            if (canReappear)
            {
                for (const auto &uuid: reappearUuids)
                {
                    if (uuid->toString() == str)
                    {
                        return uuid;
                    }
                }

            }
            else
            {
                for (const auto &uuid: uuids)
                {
                    if (uuid->toString() == str)
                    {
                        return uuid;
                    }
                }

            }

            auto newUuid = std::make_shared<UUID>(str, true);
            if (canReappear)reappearUuids.insert(newUuid);
            else uuids.insert(newUuid);
            return newUuid;
        }
    };
}

#endif //DEMONENGINE_UUIDMANAGER_HPP
