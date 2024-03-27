//
// Created by Demon on 2024/3/13.
//

#ifndef DEMONENGINE_ICONFIGURATION_HPP
#define DEMONENGINE_ICONFIGURATION_HPP

#include <map>
#include <list>
#include <vector>
#include <optional>
#include <any>
#include <memory>
#include <string>

namespace io
{
    class IConfiguration
    {
    public:
        virtual ~IConfiguration() = default;

        virtual void set(const std::string &path, const std::any &value) = 0;

        [[nodiscard]] virtual std::optional<std::any> get(const std::string &path) const = 0;

        [[nodiscard]] virtual bool contains(const std::string &path) const = 0;

        [[nodiscard]] virtual std::shared_ptr<IConfiguration> createSection(const std::string &path) = 0;

        [[nodiscard]] virtual bool getBoolean(const std::string &path, bool def = false) const = 0;

        [[nodiscard]] virtual int getInt(const std::string &path, int def = 0) const = 0;

        [[nodiscard]] virtual double getDouble(const std::string &path, double def = 0.0) const = 0;

        [[nodiscard]] virtual float getFloat(const std::string &path, float def = 0.0) const = 0;

        [[nodiscard]] virtual std::string getString(const std::string &path, const std::string &def = "") const = 0;

        [[nodiscard]] virtual std::map<std::string, std::any> getValues(bool deep = false) const = 0;

        [[nodiscard]] virtual std::vector<std::string> getStringList(const std::string &path) const = 0;

        [[nodiscard]] virtual std::vector<int> getIntList(const std::string &path) const = 0;

        [[nodiscard]] virtual std::vector<float> getFloatList(const std::string &path) const = 0;

        [[nodiscard]] virtual std::vector<double> getDoubleList(const std::string &path) const = 0;

        [[nodiscard]] virtual std::vector<bool> getBooleanList(const std::string &path) const = 0;

        [[nodiscard]] virtual bool isInt(const std::string &path) const = 0;

        [[nodiscard]] virtual bool isFloat(const std::string &path) const = 0;

        [[nodiscard]] virtual bool isDouble(const std::string &path) const = 0;

        [[nodiscard]] virtual bool isBool(const std::string &path) const = 0;

        [[nodiscard]] virtual bool isString(const std::string &path) const = 0;
    };

}

#endif //DEMONENGINE_ICONFIGURATION_HPP
