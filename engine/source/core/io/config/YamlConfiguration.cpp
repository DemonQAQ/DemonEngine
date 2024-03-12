//
// Created by Demon on 2024/3/11.
//

#include <iostream>
#include "YamlConfiguration.hpp"

namespace io
{
    YamlConfiguration::YamlConfiguration(const std::string &path) : IConfigurable(path)
    {
        load(path);
    }

    void YamlConfiguration::load(const std::string &path)
    {
        try
        {
            configRoot = YAML::LoadFile(path);
            filePath = path;
        }
        catch(const YAML::Exception& e)
        {
            std::cerr << "Exception caught in load: " << e.what() << std::endl;
            // Handle the exception or propagate
        }
    }

    void YamlConfiguration::save(const std::string &path) const
    {

    }

    [[nodiscard]] std::optional<std::any> YamlConfiguration::get(const std::string &key) const
    {
        return std::nullopt;
    }

    void YamlConfiguration::set(const std::string &key, const std::any &value)
    {

    }

    bool YamlConfiguration::contains(const std::string &path) const
    {
        return false;
    }

    std::shared_ptr<IConfigurationSection> YamlConfiguration::createSection(const std::string &path)
    {
        return std::shared_ptr<IConfigurationSection>();
    }

    bool YamlConfiguration::getBoolean(const std::string &path, bool def) const
    {
        return false;
    }

    int YamlConfiguration::getInt(const std::string &path, int def) const
    {
        return 0;
    }

    double YamlConfiguration::getDouble(const std::string &path, double def) const
    {
        return 0;
    }

    std::string YamlConfiguration::getString(const std::string &path, const std::string &def) const
    {
        return std::string();
    }

    std::map<std::string, std::any> YamlConfiguration::getValues(bool deep) const
    {
        return std::map<std::string, std::any>();
    }

    std::vector<std::string> YamlConfiguration::getStringList(const std::string &path) const
    {
        return std::vector<std::string>();
    }

    std::vector<int> YamlConfiguration::getIntList(const std::string &path) const
    {
        return std::vector<int>();
    }

    std::vector<float> YamlConfiguration::getFloatList(const std::string &path) const
    {
        return std::vector<float>();
    }

    std::vector<double> YamlConfiguration::getDoubleList(const std::string &path) const
    {
        return std::vector<double>();
    }

    std::vector<bool> YamlConfiguration::getBooleanList(const std::string &path) const
    {
        return std::vector<bool>();
    }

    bool YamlConfiguration::isInt(const std::string &path) const
    {
        return false;
    }

    bool YamlConfiguration::isFloat(const std::string &path) const
    {
        return false;
    }

    bool YamlConfiguration::isDouble(const std::string &path) const
    {
        return false;
    }

    bool YamlConfiguration::isBool(const std::string &path) const
    {
        return false;
    }

    bool YamlConfiguration::isString(const std::string &path) const
    {
        return false;
    }

}

