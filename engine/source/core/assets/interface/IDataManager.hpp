//
// Created by Demon on 2024/3/6.
//

#ifndef DEMONENGINE_IDATAMANAGER_HPP
#define DEMONENGINE_IDATAMANAGER_HPP

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <any>
#include "core/base/common/UUID.hpp"

namespace assets
{
    class IDataManager
    {
    public:
        virtual ~IDataManager() = default;

        virtual bool loadData(const std::vector<std::any>& params) = 0;
        virtual void unloadData(const std::vector<std::any>& params) = 0;
        [[nodiscard]] virtual bool isDataLoaded(const std::vector<std::any>& params) const = 0;
        virtual void updateData(const std::vector<std::any>& params) = 0;

    protected:
        static std::vector<char> loadFile(const std::string& path)
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

        static void saveFile(const std::string& path, const std::vector<char>& data)
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

#endif //DEMONENGINE_IDATAMANAGER_HPP
