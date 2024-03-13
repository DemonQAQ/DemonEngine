﻿//
// Created by Demon on 2024/3/11.
//

#include <iostream>
#include <fstream>
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
        catch (const YAML::Exception &e)
        {
            std::cerr << "Exception caught in load: " << e.what() << std::endl;
            // Handle the exception or propagate
        }
    }

    void YamlConfiguration::save(const std::string &path) const
    {
        std::ofstream fout(path);
        fout << YAML::Dump(configRoot);
    }

    [[nodiscard]] std::optional<std::any> YamlConfiguration::get(const std::string &key) const
    {
        if (!configRoot[key]) return std::nullopt;
        const YAML::Node &node = configRoot[key];

        if (node.IsScalar())
        {
            return std::make_optional<std::any>(node.Scalar());
        } else if (node.IsSequence())
        {
            std::vector<std::any> sequence;
            for (const auto &item: node)
            {
                if (item.IsScalar())sequence.push_back(item.Scalar());
            }
            return std::make_optional<std::any>(sequence);
        } else if (node.IsMap())
        {
            std::map<std::string, std::any> map;
            for (const auto &pair: node)
            {
                if (pair.first.IsScalar())
                {
                    std::string key_ = pair.first.Scalar();
                    if (pair.second.IsScalar())map[key_] = pair.second.Scalar();
                }
            }
            return std::make_optional<std::any>(map);
        }
        return std::nullopt;
    }

    void YamlConfiguration::set(const std::string &key, const std::any &value)
    {
        try
        {
            if (value.type() == typeid(int))
            {
                configRoot[key] = std::any_cast<int>(value);
            } else if (value.type() == typeid(double))
            {
                configRoot[key] = std::any_cast<double>(value);
            } else if (value.type() == typeid(bool))
            {
                configRoot[key] = std::any_cast<bool>(value);
            } else if (value.type() == typeid(std::string))
            {
                configRoot[key] = std::any_cast<std::string>(value);
            }
        } catch (const std::bad_any_cast &e)
        {
            std::cerr << "类型转换错误: " << e.what() << std::endl;
        }
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
        if (configRoot[path] && configRoot[path].IsScalar())
        {
            try
            {
                return configRoot[path].as<bool>();
            } catch (const YAML::BadConversion &e)
            {
                std::cerr << "Bad conversion for getInt: " << e.what() << std::endl;
                return def;
            }
        }
        return def;
    }

    bool YamlConfiguration::getBoolean(const std::string &path) const
    {
        return getBoolean(path, false);
    }

    int YamlConfiguration::getInt(const std::string &path, int def) const
    {
        if (configRoot[path] && configRoot[path].IsScalar())
        {
            try
            {
                return configRoot[path].as<int>();
            } catch (const YAML::BadConversion &e)
            {
                std::cerr << "Bad conversion for getInt: " << e.what() << std::endl;
                return def;
            }
        }
        return def;
    }

    int YamlConfiguration::getInt(const std::string &path) const
    {
        return getInt(path, 0);
    }

    float YamlConfiguration::getFloat(const std::string &path, float def) const
    {
        if (configRoot[path] && configRoot[path].IsScalar())
        {
            try
            {
                return configRoot[path].as<float>();
            } catch (const YAML::BadConversion &e)
            {
                std::cerr << "Bad conversion for getInt: " << e.what() << std::endl;
                return def;
            }
        }
        return def;
    }

    float YamlConfiguration::getFloat(const std::string &path) const
    {
        return getFloat(path, 0.0);
    }

    double YamlConfiguration::getDouble(const std::string &path, double def) const
    {
        if (configRoot[path] && configRoot[path].IsScalar())
        {
            try
            {
                return configRoot[path].as<double>();
            } catch (const YAML::BadConversion &e)
            {
                std::cerr << "Bad conversion for getInt: " << e.what() << std::endl;
                return def;
            }
        }
        return def;
    }

    double YamlConfiguration::getDouble(const std::string &path) const
    {
        return getDouble(path, 0);
    }

    std::string YamlConfiguration::getString(const std::string &path, const std::string &def) const
    {
        if (configRoot[path] && configRoot[path].IsScalar())
        {
            try
            {
                return configRoot[path].as<std::string>();
            } catch (const YAML::BadConversion &e)
            {
                std::cerr << "Bad conversion for getInt: " << e.what() << std::endl;
                return def;
            }
        }
        return def;
    }

    std::string YamlConfiguration::getString(const std::string &path) const
    {
        return getString(path, "");
    }

    std::map<std::string, std::any> YamlConfiguration::getValues(bool deep) const
    {
        std::map<std::string, std::any> values;
        std::function<void(const YAML::Node &, const std::string &)> parseNode;
        parseNode = [&](const YAML::Node &node, const std::string &prefix)
        {
            for (const auto &element: node)
            {
                std::string key = prefix.empty() ? element.first.as<std::string>() : prefix + "." +
                                                                                     element.first.as<std::string>();
                if (element.second.IsScalar())
                {
                    values[key] = element.second.Scalar();
                } else if (element.second.IsSequence())
                {
                    std::vector<std::any> list;
                    for (const auto &listItem: element.second)
                    {
                        if (listItem.IsScalar())
                        {
                            list.emplace_back(listItem.Scalar());
                        } else
                        {
                            // For simplicity, this example does not handle nested sequences or maps
                            // You might want to extend this to handle more complex structures
                        }
                    }
                    values[key] = list;
                } else if (deep && element.second.IsMap())parseNode(element.second, key);
            }
        };
        parseNode(configRoot, "");
        return values;
    }

    std::map<std::string, std::any> YamlConfiguration::getValues() const
    {
        return getValues(false);
    }

    std::vector<std::string> YamlConfiguration::getStringList(const std::string &path) const
    {
        std::vector<std::string> list;
        if (configRoot[path] && configRoot[path].IsSequence())
        {
            for (const auto &element: configRoot[path])
            {
                list.push_back(element.as<std::string>());
            }
        }
        return list;
    }

    std::vector<int> YamlConfiguration::getIntList(const std::string &path) const
    {
        std::vector<int> list;
        if (configRoot[path] && configRoot[path].IsSequence())
        {
            for (const auto &element: configRoot[path])
            {
                list.push_back(element.as<int>());
            }
        }
        return list;
    }

    std::vector<float> YamlConfiguration::getFloatList(const std::string &path) const
    {
        std::vector<float> list;
        if (configRoot[path] && configRoot[path].IsSequence())
        {
            for (const auto &element: configRoot[path])
            {
                list.push_back(element.as<float>());
            }
        }
        return list;
    }

    std::vector<double> YamlConfiguration::getDoubleList(const std::string &path) const
    {
        std::vector<double> list;
        if (configRoot[path] && configRoot[path].IsSequence())
        {
            for (const auto &element: configRoot[path])
            {
                list.push_back(element.as<double>());
            }
        }
        return list;
    }

    std::vector<bool> YamlConfiguration::getBooleanList(const std::string &path) const
    {
        std::vector<bool> list;
        if (configRoot[path] && configRoot[path].IsSequence())
        {
            for (const auto &element: configRoot[path])
            {
                list.push_back(element.as<bool>());
            }
        }
        return list;
    }

    bool YamlConfiguration::isInt(const std::string &path) const
    {
        if (!configRoot[path] || !configRoot[path].IsScalar())
        {
            return false;
        }
        try
        {
            configRoot[path].as<int>();
            return true;
        } catch (const YAML::BadConversion &e)
        {
            return false;
        }
    }

    bool YamlConfiguration::isFloat(const std::string &path) const
    {
        if (!configRoot[path] || !configRoot[path].IsScalar())
        {
            return false;
        }
        try
        {
            configRoot[path].as<float>();
            return true;
        } catch (const YAML::BadConversion &e)
        {
            return false;
        }
    }

    bool YamlConfiguration::isDouble(const std::string &path) const
    {
        if (!configRoot[path] || !configRoot[path].IsScalar())
        {
            return false;
        }
        try
        {
            configRoot[path].as<double>();
            return true;
        } catch (const YAML::BadConversion &e)
        {
            return false;
        }
    }

    bool YamlConfiguration::isBool(const std::string &path) const
    {
        if (!configRoot[path] || !configRoot[path].IsScalar())
        {
            return false;
        }
        try
        {
            configRoot[path].as<bool>();
            return true;
        } catch (const YAML::BadConversion &e)
        {
            return false;
        }
    }

    bool YamlConfiguration::isString(const std::string &path) const
    {
        if (!configRoot[path] || !configRoot[path].IsScalar())
        {
            return false;
        }
        try
        {
            configRoot[path].as<std::string>();
            return true;
        } catch (const YAML::BadConversion &e)
        {
            return false;
        }
    }

}

