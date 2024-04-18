//
// Created by Demon on 2024/4/13.
//

#ifndef DEMONENGINE_CONFIGLOADER_HPP
#define DEMONENGINE_CONFIGLOADER_HPP

#include <core/io/FileType.hpp>
#include <core/base/interface/Interface.hpp>
#include <core/assets/interface/IFileLoader.hpp>
#include <string>
#include <memory>
#include <core/io/config/YamlConfiguration.hpp>
#include <core/io/config/XmlConfiguration.hpp>
#include <core/io/config/JsonConfiguration.hpp>

namespace assets
{
    class ConfigLoader : implements IFileLoader
    {
    public:
        static std::shared_ptr<io::YamlConfiguration>loadYml(const std::string &path, bool isYml = false, bool isFullPath = false);

        static std::shared_ptr<io::XmlConfiguration> loadXml(const std::string &path, bool isXml = false, bool isFullPath = false);

        static std::shared_ptr<io::JsonConfiguration> loadJson(const std::string &path, bool isJson = false, bool isFullPath = false);

        static io::FileType getFileType(const std::string &path);
    };
}

#endif //DEMONENGINE_CONFIGLOADER_HPP
