//
// Created by Demon on 2024/4/9.
//

#ifndef DEMONENGINE_IMETAACCESSOR_HPP
#define DEMONENGINE_IMETAACCESSOR_HPP

#include <memory>
#include <core/base/common/UUID.hpp>
#include "Interface.hpp"
#include "core/base/common/Metadata.hpp"
#include "BlockOperator.hpp"

namespace base
{
    class NormalBlockOperators : implements BlockOperator
    {
    public:
        void writeToBlock(std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml) override
        {
            yml->set("uuid", metadata->getValue("uuid"));
        }

        void
        readFromBlock(std::shared_ptr<Metadata> &metadata, std::shared_ptr<io::YamlConfiguration> &yml) override
        {
            metadata->setValue("uuid", yml->getString("uuid"));
        }

        void initBlock(std::shared_ptr<Metadata> &metadata, const std::vector<std::any> &params) override
        {
            if (params.empty())
            {
                metadata->setValue("uuid", "none");
                return;
            }

            std::string uuidValue;
            if (params[0].type() == typeid(std::shared_ptr<base::UUID>))
            {
                auto uuidPtr = std::any_cast<std::shared_ptr<base::UUID>>(params[0]);
                uuidValue = uuidPtr->toString();
            }
            else uuidValue = "none";
            metadata->setValue("uuid", uuidValue);
        }
    };

    class IMetaAccessor
    {
    private:
        std::shared_ptr<Metadata> metadata;
    protected:
        static NormalBlockOperators normalBlockOperators;
        std::vector<std::shared_ptr<BlockOperator>> operators = {};
    public:
        IMetaAccessor(std::shared_ptr<io::YamlConfiguration> &yml, bool needInit,
                      const std::shared_ptr<base::UUID> &uuid = nullptr)
        {
            metadata = std::make_shared<Metadata>();
            init();
            if (needInit)
            {
                normalBlockOperators.initBlock(metadata, {uuid});
                normalBlockOperators.writeToBlock(metadata, yml);
                for (auto &anOperator: operators)
                {
                    anOperator->initBlock(metadata, {});
                    anOperator->writeToBlock(metadata, yml);
                }
            }
            else
            {
                normalBlockOperators.readFromBlock(metadata, yml);
                for (auto &anOperator: operators)anOperator->readFromBlock(metadata, yml);
            }
        }

        void init(){}

        bool addOperator(const std::shared_ptr<BlockOperator> &newOperator)
        {
            auto it = std::find_if(operators.begin(), operators.end(),
                                   [&newOperator](const std::shared_ptr<BlockOperator> &operator_)
                                   {
                                       return operator_.get() == newOperator.get();
                                   });

            if (it == operators.end())
            {
                operators.push_back(newOperator);
                return true;
            }
            return false;
        }

        bool removeOperator(const std::shared_ptr<BlockOperator> &operatorToRemove)
        {
            auto originalSize = operators.size();
            operators.erase(
                    std::remove_if(operators.begin(), operators.end(),
                                   [&operatorToRemove](const std::shared_ptr<BlockOperator> &operator_)
                                   {
                                       return operator_.get() == operatorToRemove.get();
                                   }), operators.end());

            return originalSize != operators.size();
        }

        [[nodiscard]] const std::shared_ptr<Metadata> &getMetadata() const
        {
            return metadata;
        }

        virtual void setMetadata(std::shared_ptr<Metadata> newMetadata)
        {
            metadata = std::move(newMetadata);
        }

        void setMetadataValue(const std::string &key, const std::any &value)
        {
            if (!metadata)
            {
                metadata = std::make_shared<Metadata>();
            }
            metadata->setValue(key, value);
        }

        [[nodiscard]] std::any getMetadataValue(const std::string &key) const
        {
            if (metadata)
            {
                return metadata->getValue(key);
            }
            return {};
        }

        [[nodiscard]] bool containsMetadataKey(const std::string &key) const
        {
            if (metadata)
            {
                return metadata->containsKey(key);
            }
            return false;
        }
    };
}

#endif //DEMONENGINE_IMETAACCESSOR_HPP
