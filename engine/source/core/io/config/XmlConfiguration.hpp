﻿//
// Created by Demon on 2024/3/12.
//

#ifndef DEMONENGINE_XMLCONFIGURATION_HPP
#define DEMONENGINE_XMLCONFIGURATION_HPP

#include "core/base/interface/Interface.hpp"
#include "core/io/interface/IFile.hpp"
#include "core/io/interface/IConfiguration.hpp"

namespace io
{
    class XmlConfiguration : implements IFile, IConfiguration
    {
    private:
        boost::property_tree::ptree tree;
    public:
        void load(const std::string &path) override;

        void save(const std::string &path) const override;

        [[nodiscard]] std::optional<std::any> get(const std::string &key) const override;

        void set(const std::string &key, const std::any &value) override;

        explicit XmlConfiguration(const std::string &path);

        [[nodiscard]] bool contains(const std::string &path) const override;

        [[nodiscard]] std::shared_ptr<IConfiguration> createSection(const std::string &path) override;

        [[nodiscard]] bool getBoolean(const std::string &path, bool def) const override;

        [[nodiscard]] bool getBoolean(const std::string &path) const;

        [[nodiscard]] int getInt(const std::string &path, int def) const override;

        [[nodiscard]] int getInt(const std::string &path) const;

        [[nodiscard]] float getFloat(const std::string &path, float def) const override;

        [[nodiscard]] float getFloat(const std::string &path) const;

        [[nodiscard]] double getDouble(const std::string &path, double def) const override;

        [[nodiscard]] double getDouble(const std::string &path) const;

        [[nodiscard]] std::string getString(const std::string &path, const std::string &def) const override;

        [[nodiscard]] std::string getString(const std::string &path) const;

        [[nodiscard]] std::map<std::string, std::any> getValues(bool deep) const override;

        [[nodiscard]] std::map<std::string, std::any> getValues() const;

        [[nodiscard]] std::vector<std::string> getStringList(const std::string &path) const override;

        [[nodiscard]] std::vector<int> getIntList(const std::string &path) const override;

        [[nodiscard]] std::vector<float> getFloatList(const std::string &path) const override;

        [[nodiscard]] std::vector<double> getDoubleList(const std::string &path) const override;

        [[nodiscard]] std::vector<bool> getBooleanList(const std::string &path) const override;

        [[nodiscard]] bool isInt(const std::string &path) const override;

        [[nodiscard]] bool isFloat(const std::string &path) const override;

        [[nodiscard]] bool isDouble(const std::string &path) const override;

        [[nodiscard]] bool isBool(const std::string &path) const override;

        [[nodiscard]] bool isString(const std::string &path) const override;
    };
}

#endif //DEMONENGINE_XMLCONFIGURATION_HPP
