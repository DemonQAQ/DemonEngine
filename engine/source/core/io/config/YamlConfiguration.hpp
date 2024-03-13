//
// Created by Demon on 2024/3/11.
//

#ifndef DEMONENGINE_YAMLCONFIGURATION_HPP
#define DEMONENGINE_YAMLCONFIGURATION_HPP

#include "core/base/interface/Interface.hpp"
#include "core/io/interface/IConfigurable.hpp"
#include "core/io/interface/IConfigurationSection.hpp"
#include <yaml-cpp/yaml.h>

namespace io
{
    class YamlConfiguration : implements IConfigurable, IConfigurationSection
    {
    private:
        YAML::Node configRoot;
        std::string filePath;

    public:
        void load(const std::string &path) override;

        void save(const std::string &path) const override;

        [[nodiscard]] std::optional<std::any> get(const std::string &key) const override;

        void set(const std::string &key, const std::any &value) override;

        explicit YamlConfiguration(const std::string &path);

        [[nodiscard]] bool contains(const std::string &path) const override;

        [[nodiscard]] std::shared_ptr<IConfigurationSection> createSection(const std::string &path) override;

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

#endif //DEMONENGINE_YAMLCONFIGURATION_HPP
