//
// Created by Demon on 2024/3/12.
//

#include <iostream>
#include "JsonConfiguration.hpp"

namespace io
{
    JsonConfiguration::JsonConfiguration(const std::shared_ptr<base::UUID> &existingUuid, const std::string &path) : IFile(existingUuid,path)
    {
        load();
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

    void JsonConfiguration::save() const
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

    void JsonConfiguration::load()
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
        return tree.get_optional<std::string>(path).has_value();
    }

    std::shared_ptr<IConfiguration> JsonConfiguration::createSection(const std::string &path)
    {
        return std::shared_ptr<IConfiguration>();
    }

    bool JsonConfiguration::getBoolean(const std::string &path, bool def) const
    {
        return tree.get(path, def);
    }

    bool JsonConfiguration::getBoolean(const std::string &path) const
    {
        return getBoolean(path, false);
    }

    int JsonConfiguration::getInt(const std::string &path, int def) const
    {
        return tree.get(path, def);
    }

    int JsonConfiguration::getInt(const std::string &path) const
    {
        return getInt(path, 0);
    }

    double JsonConfiguration::getDouble(const std::string &path, double def) const
    {
        return tree.get(path, def);
    }

    double JsonConfiguration::getDouble(const std::string &path) const
    {
        return getDouble(path, 0.0);
    }

    float JsonConfiguration::getFloat(const std::string &path, float def) const
    {
        return tree.get(path, def);
    }

    float JsonConfiguration::getFloat(const std::string &path) const
    {
        return getFloat(path,0.0);
    }

    std::string JsonConfiguration::getString(const std::string &path, const std::string &def) const
    {
        return tree.get(path, def);
    }

    std::string JsonConfiguration::getString(const std::string &path) const
    {
        return getString(path, "");
    }

    std::map<std::string, std::any> JsonConfiguration::getValues(bool deep) const
    {
        std::map<std::string, std::any> values;

        std::function<void(const boost::property_tree::ptree &, const std::string &)> parseNode =
                [&](const boost::property_tree::ptree &node, const std::string &base_path)
                {
                    for (const auto &it: node)
                    {
                        std::string path = base_path.empty() ? it.first : base_path + "." + it.first;

                        if (it.second.empty())
                        {
                            values[path] = it.second.data();
                        } else if (it.second.front().first.empty())
                        {
                            std::vector<std::any> list;
                            for (const auto &list_it: it.second)
                            {
                                if (list_it.second.empty())list.emplace_back(list_it.second.data());
                                else{}
                            }
                            values[path] = list;
                        } else
                        {
                            if (deep)
                            {
                                std::map<std::string, std::any> sub_values = getValues(true);
                                values[path] = sub_values;
                            }
                        }
                    }
                };

        parseNode(tree, "");
        return values;
    }

    std::map<std::string, std::any> JsonConfiguration::getValues() const
    {
        return getValues(false);
    }

    std::vector<std::string> JsonConfiguration::getStringList(const std::string &path) const
    {
        std::vector<std::string> list;
        for (const auto &it: tree.get_child(path))
        {
            list.push_back(it.second.data());
        }
        return list;
    }

    std::vector<int> JsonConfiguration::getIntList(const std::string &path) const
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

    std::vector<float> JsonConfiguration::getFloatList(const std::string &path) const
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

    std::vector<double> JsonConfiguration::getDoubleList(const std::string &path) const
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

    std::vector<bool> JsonConfiguration::getBooleanList(const std::string &path) const
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

    bool JsonConfiguration::isInt(const std::string &path) const
    {
        auto opt = tree.get_optional<int>(path);
        return opt.has_value();
    }

    bool JsonConfiguration::isFloat(const std::string &path) const
    {
        auto opt = tree.get_optional<float>(path);
        return opt.has_value();
    }

    bool JsonConfiguration::isDouble(const std::string &path) const
    {
        auto opt = tree.get_optional<double>(path);
        return opt.has_value();
    }

    bool JsonConfiguration::isBool(const std::string &path) const
    {
        auto opt = tree.get_optional<bool>(path);
        return opt.has_value();
    }

    bool JsonConfiguration::isString(const std::string &path) const
    {
        auto opt = tree.get_optional<std::string>(path);
        return opt.has_value();
    }

    bool JsonConfiguration::isEmpty() const
    {
        return tree.empty();
    }

    std::shared_ptr<char[]> JsonConfiguration::toCharList() const
    {
        return nullptr;
    }

}


