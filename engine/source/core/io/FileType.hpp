//
// Created by Demon on 2024/3/18.
//

#ifndef DEMONENGINE_FILETYPE_HPP
#define DEMONENGINE_FILETYPE_HPP

#include <string>

namespace io
{
    enum class FileType
    {
        YAML,
        JSON,
        XML,
        NORMAL
    };

    inline const char *toString(io::FileType type)
    {
        switch (type)
        {
            case FileType::YAML:
                return ".yml";
            case FileType::JSON:
                return ".json";
            case FileType::XML:
                return ".xml";
            case FileType::NORMAL:
            default:
                return ".*";
        }
    }

    static io::FileType fromStringToFileType(const std::string &typeName)
    {
        if (typeName == ".yml") return FileType::YAML;
        else if (typeName == ".meta") return FileType::YAML;
        else if (typeName == ".json") return FileType::JSON;
        else if (typeName == ".xml") return FileType::XML;
        else return FileType::NORMAL;
    }
}
#endif //DEMONENGINE_FILETYPE_HPP
