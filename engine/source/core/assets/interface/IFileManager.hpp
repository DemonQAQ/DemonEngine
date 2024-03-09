//
// Created by Demon on 2024/3/6.
//

#ifndef DEMONENGINE_IFILEMANAGER_HPP
#define DEMONENGINE_IFILEMANAGER_HPP

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <any>
#include "core/base/common/UUID.hpp"

namespace assets
{
    class IFileManager
    {
    public:
        virtual ~IFileManager() = default;

        virtual std::optional<base::UUID> LoadResource(const std::vector<std::any>& params) = 0;
        virtual void UnloadResource(const std::vector<std::any>& params) = 0;
        [[nodiscard]] virtual bool IsResourceLoaded(const std::vector<std::any>& params) const = 0;
        virtual void UpdateResource(const std::vector<std::any>& params) = 0;

    protected:
        static std::vector<char> LoadFile(const std::string& path)
        {
            std::ifstream file(path, std::ios::binary | std::ios::ate);
            if (!file)
            {
                std::cerr << "Unable to open file: " << path << std::endl;
                return {};
            }

            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<char> buffer(static_cast<size_t>(size));
            if (!file.read(buffer.data(), size))
            {
                std::cerr << "Failed to read file: " << path << std::endl;
                return {};
            }

            return buffer;
        }

        static void SaveFile(const std::string& path, const std::vector<char>& data)
        {
            std::ofstream file(path, std::ios::binary);
            if (!file)
            {
                std::cerr << "Unable to open file for writing: " << path << std::endl;
                return;
            }

            file.write(data.data(), static_cast<std::streamsize>(data.size()));
            if (!file)
            {
                std::cerr << "Failed to write file: " << path << std::endl;
            }
        }
    };
}

#endif //DEMONENGINE_IFILEMANAGER_HPP
