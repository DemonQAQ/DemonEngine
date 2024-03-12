//
// Created by Demon on 2024/3/12.
//

#include <iostream>
#include "JsonConfiguration.hpp"

namespace io
{
    JsonConfiguration::JsonConfiguration(const std::string &path) : IConfigurable(path)
    {
        load(path);
    }

    void JsonConfiguration::set(const std::string &key, const std::any &value)
    {
        try
        {
            auto valStr = std::any_cast<std::string>(value);
            tree.put(key, valStr);
        }
        catch (const std::bad_any_cast &)
        {
            std::cerr << "Invalid type for JSON configuration value." << std::endl;
        }
    }

    std::optional<std::any> JsonConfiguration::get(const std::string &key) const
    {
        try
        {
            auto value = tree.get<std::string>(key);
            return std::make_optional<std::any>(value);
        }
        catch (const boost::property_tree::ptree_bad_path &)
        {
            return std::nullopt;
        }
    }

    void JsonConfiguration::save(const std::string &path) const
    {
        try
        {
            boost::property_tree::write_json(path, tree);
        }
        catch (const boost::property_tree::json_parser_error &e)
        {
            std::cerr << "Error writing JSON configuration: " << e.what() << std::endl;
        }
    }

    void JsonConfiguration::load(const std::string &path)
    {
        try
        {
            boost::property_tree::read_json(path, tree);
        }
        catch (const boost::property_tree::json_parser_error &e)
        {
            std::cerr << "Error reading JSON configuration: " << e.what() << std::endl;
        }
    }

    bool JsonConfiguration::contains(const std::string &path) const
    {
        return false;
    }

    std::shared_ptr<IConfigurationSection> JsonConfiguration::createSection(const std::string &path)
    {
        return std::shared_ptr<IConfigurationSection>();
    }

    bool JsonConfiguration::getBoolean(const std::string &path, bool def) const
    {
        return false;
    }

    int JsonConfiguration::getInt(const std::string &path, int def) const
    {
        return 0;
    }

    double JsonConfiguration::getDouble(const std::string &path, double def) const
    {
        return 0;
    }

    std::string JsonConfiguration::getString(const std::string &path, const std::string &def) const
    {
        return std::string();
    }

    std::map<std::string, std::any> JsonConfiguration::getValues(bool deep) const
    {
        return std::map<std::string, std::any>();
    }

    std::vector<std::string> JsonConfiguration::getStringList(const std::string &path) const
    {
        return std::vector<std::string>();
    }

    std::vector<int> JsonConfiguration::getIntList(const std::string &path) const
    {
        return std::vector<int>();
    }

    std::vector<float> JsonConfiguration::getFloatList(const std::string &path) const
    {
        return std::vector<float>();
    }

    std::vector<double> JsonConfiguration::getDoubleList(const std::string &path) const
    {
        return std::vector<double>();
    }

    std::vector<bool> JsonConfiguration::getBooleanList(const std::string &path) const
    {
        return std::vector<bool>();
    }

    bool JsonConfiguration::isInt(const std::string &path) const
    {
        return false;
    }

    bool JsonConfiguration::isFloat(const std::string &path) const
    {
        return false;
    }

    bool JsonConfiguration::isDouble(const std::string &path) const
    {
        return false;
    }

    bool JsonConfiguration::isBool(const std::string &path) const
    {
        return false;
    }

    bool JsonConfiguration::isString(const std::string &path) const
    {
        return false;
    }
}


