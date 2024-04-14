//
// Created by Demon on 2024/3/12.
//

#include <iostream>
#include "XmlConfiguration.hpp"

namespace io
{
    XmlConfiguration::XmlConfiguration(const std::shared_ptr<base::UUID> &existingUuid, const std::string &path) : IFile(existingUuid, path)
    {
        load();
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
        catch (const boost::property_tree::ptree_bad_path &e)
        {
            std::cerr << "Error casting value for XML configuration: " << e.what() << std::endl;
        }
        return std::nullopt;
    }

    void XmlConfiguration::save() const
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

    void XmlConfiguration::load()
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
        return tree.get_optional<std::string>(path).has_value();
    }

    std::shared_ptr<IConfiguration> XmlConfiguration::createSection(const std::string &path)
    {
        return std::shared_ptr<IConfiguration>();
    }

    bool XmlConfiguration::getBoolean(const std::string &path, bool def) const
    {
        return tree.get(path, def);
    }

    bool XmlConfiguration::getBoolean(const std::string &path) const
    {
        return getBoolean(path, false);
    }

    int XmlConfiguration::getInt(const std::string &path, int def) const
    {
        return tree.get(path, def);
    }

    int XmlConfiguration::getInt(const std::string &path) const
    {
        return getInt(path, 0);
    }

    double XmlConfiguration::getDouble(const std::string &path, double def) const
    {
        return tree.get(path, def);
    }

    double XmlConfiguration::getDouble(const std::string &path) const
    {
        return getDouble(path, 0.0);
    }

    float XmlConfiguration::getFloat(const std::string &path, float def) const
    {
        return tree.get(path, def);
    }

    float XmlConfiguration::getFloat(const std::string &path) const
    {
        return getFloat(path, 0.0);
    }

    std::string XmlConfiguration::getString(const std::string &path, const std::string &def) const
    {
        return tree.get(path, def);
    }

    std::string XmlConfiguration::getString(const std::string &path) const
    {
        return getString(path, "");
    }

    std::map<std::string, std::any> XmlConfiguration::getValues(bool deep) const
    {
        std::map<std::string, std::any> values;
        std::function<void(const boost::property_tree::ptree &, const std::string &)> parseNode;

        parseNode = [&](const boost::property_tree::ptree &node, const std::string &path)
        {
            for (const auto &it: node)
            {
                std::string currentPath = path.empty() ? it.first : path + "." + it.first;

                if (it.second.empty())
                {
                    values[currentPath] = std::any(it.second.data());
                } else if (deep)
                {
                    if (it.second.size() == 1 && it.second.front().first.empty())
                    {
                        std::vector<std::any> list;
                        for (const auto &listIt: it.second)
                        {
                            list.emplace_back(listIt.second.data());
                        }
                        values[currentPath] = std::any(list);
                    } else parseNode(it.second, currentPath);
                }
            }
        };

        parseNode(tree, "");
        return values;
    }

    std::map<std::string, std::any> XmlConfiguration::getValues() const
    {
        return getValues(false);
    }

    std::vector<std::string> XmlConfiguration::getStringList(const std::string &path) const
    {
        std::vector<std::string> list;
        for (auto &it: tree.get_child(path))
        {
            list.push_back(it.second.data());
        }
        return list;
    }

    std::vector<int> XmlConfiguration::getIntList(const std::string &path) const
    {
        std::vector<int> list;
        try
        {
            for (auto &it: tree.get_child(path))
            {
                list.push_back(it.second.get_value<int>());
            }
        } catch (const boost::property_tree::ptree_error &e)
        {
            std::cerr << "Error accessing or converting to int list: " << e.what() << std::endl;
        }
        return list;
    }

    std::vector<float> XmlConfiguration::getFloatList(const std::string &path) const
    {
        std::vector<float> list;
        try
        {
            for (auto &it: tree.get_child(path))
            {
                list.push_back(it.second.get_value<float>());
            }
        } catch (const boost::property_tree::ptree_error &e)
        {
            std::cerr << "Error accessing or converting to int list: " << e.what() << std::endl;
        }
        return list;
    }

    std::vector<double> XmlConfiguration::getDoubleList(const std::string &path) const
    {
        std::vector<double> list;
        try
        {
            for (auto &it: tree.get_child(path))
            {
                list.push_back(it.second.get_value<double>());
            }
        } catch (const boost::property_tree::ptree_error &e)
        {
            std::cerr << "Error accessing or converting to int list: " << e.what() << std::endl;
        }
        return list;
    }

    std::vector<bool> XmlConfiguration::getBooleanList(const std::string &path) const
    {
        std::vector<bool> list;
        try
        {
            for (auto &it: tree.get_child(path))
            {
                list.push_back(it.second.get_value<bool>());
            }
        } catch (const boost::property_tree::ptree_error &e)
        {
            std::cerr << "Error accessing or converting to int list: " << e.what() << std::endl;
        }
        return list;
    }

    bool XmlConfiguration::isInt(const std::string &path) const
    {
        auto opt = tree.get_optional<int>(path);
        return opt.has_value();
    }

    bool XmlConfiguration::isFloat(const std::string &path) const
    {
        auto opt = tree.get_optional<float>(path);
        return opt.has_value();
    }

    bool XmlConfiguration::isDouble(const std::string &path) const
    {
        auto opt = tree.get_optional<double>(path);
        return opt.has_value();
    }

    bool XmlConfiguration::isBool(const std::string &path) const
    {
        auto opt = tree.get_optional<bool>(path);
        return opt.has_value();
    }

    bool XmlConfiguration::isString(const std::string &path) const
    {
        auto opt = tree.get_optional<std::string>(path);
        return opt.has_value();
    }

    bool XmlConfiguration::isEmpty() const
    {
        return tree.empty();
    }
}


