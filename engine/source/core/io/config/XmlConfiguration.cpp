//
// Created by Demon on 2024/3/12.
//

#include <iostream>
#include "XmlConfiguration.hpp"

namespace io
{
    XmlConfiguration::XmlConfiguration(const std::string &path) : IConfigurable(path)
    {
        load(path);
    }

    void XmlConfiguration::set(const std::string &key, const std::any &value)
    {
        try
        {
            tree.put(key, std::any_cast<std::string>(value));
        }
        catch (const std::bad_any_cast &e)
        {
            std::cerr << "Error casting value for XML configuration: " << e.what() << std::endl;
        }
    }

    std::optional<std::any> XmlConfiguration::get(const std::string &key) const
    {
        try
        {
            auto val = tree.get<std::string>(key, ""); // 默认返回空字符串
            if (!val.empty())
            {
                return std::make_any<std::string>(val);
            }
        }
        catch (const boost::property_tree::ptree_bad_path& e)
        {
            std::cerr << "Error casting value for XML configuration: " << e.what() << std::endl;
        }
        return std::nullopt;
    }

    void XmlConfiguration::save(const std::string &path) const
    {
        try
        {
            boost::property_tree::write_xml(path, tree);
        }
        catch (const boost::property_tree::xml_parser::xml_parser_error &e)
        {
            std::cerr << "Error writing XML configuration: " << e.what() << std::endl;
        }
    }

    void XmlConfiguration::load(const std::string &path)
    {
        try
        {
            boost::property_tree::read_xml(path, tree);
        }
        catch (const boost::property_tree::xml_parser::xml_parser_error &e)
        {
            std::cerr << "Error reading XML configuration: " << e.what() << std::endl;
        }
    }

    bool XmlConfiguration::contains(const std::string &path) const
    {
        return false;
    }

    std::shared_ptr<IConfigurationSection> XmlConfiguration::createSection(const std::string &path)
    {
        return std::shared_ptr<IConfigurationSection>();
    }

    bool XmlConfiguration::getBoolean(const std::string &path, bool def) const
    {
        return false;
    }

    int XmlConfiguration::getInt(const std::string &path, int def) const
    {
        return 0;
    }

    double XmlConfiguration::getDouble(const std::string &path, double def) const
    {
        return 0;
    }

    std::string XmlConfiguration::getString(const std::string &path, const std::string &def) const
    {
        return std::string();
    }

    std::map<std::string, std::any> XmlConfiguration::getValues(bool deep) const
    {
        return std::map<std::string, std::any>();
    }

    std::vector<std::string> XmlConfiguration::getStringList(const std::string &path) const
    {
        return std::vector<std::string>();
    }

    std::vector<int> XmlConfiguration::getIntList(const std::string &path) const
    {
        return std::vector<int>();
    }

    std::vector<float> XmlConfiguration::getFloatList(const std::string &path) const
    {
        return std::vector<float>();
    }

    std::vector<double> XmlConfiguration::getDoubleList(const std::string &path) const
    {
        return std::vector<double>();
    }

    std::vector<bool> XmlConfiguration::getBooleanList(const std::string &path) const
    {
        return std::vector<bool>();
    }

    bool XmlConfiguration::isInt(const std::string &path) const
    {
        return false;
    }

    bool XmlConfiguration::isFloat(const std::string &path) const
    {
        return false;
    }

    bool XmlConfiguration::isDouble(const std::string &path) const
    {
        return false;
    }

    bool XmlConfiguration::isBool(const std::string &path) const
    {
        return false;
    }

    bool XmlConfiguration::isString(const std::string &path) const
    {
        return false;
    }
}


