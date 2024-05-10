//
// Created by Demon on 2023/7/30.
//

#ifndef DEMONENGINE_FILESYSTEM_HPP
#define DEMONENGINE_FILESYSTEM_HPP

#include <string>
#include <cstdlib>
static char const * logRoot = LOG_ROOT_PATH;
static char const * assetsRootPath = ASSETS_ROOT_PATH;
static char const * resourcePackRootPath = RESOURCE_PACK_ROOT_PATH;

class FileSystem
{
private:
    typedef std::string (*Builder) (const std::string& path);

public:
    static std::string getPath(const std::string& path)
    {
        static std::string(*pathBuilder)(std::string const &) = getPathBuilder();
        return (*pathBuilder)(path);
    }

    static std::string extractDirectory(const std::string filePath)
    {
        return filePath.substr(0, filePath.find_last_of('/'));
    }

    static std::string getAssetsFullPath(const std::string& path)
    {
        std::string fullPath = std::string(assetsRootPath) + path;
        return fullPath;
    }

    static std::string getResourcePackFullPath(const std::string& path)
    {
        std::string fullPath = std::string(resourcePackRootPath) + path;
        return fullPath;
    }

    static std::string combinePaths(const std::string& path1, const std::string& path2)
    {
        std::string combinedPath = path1;
        // 确保第一个路径以斜杠结尾
        if (!combinedPath.empty() && combinedPath.back() != '/')
        {
            combinedPath += "/";
        }
        // 如果第二个路径以斜杠开头，去掉这个斜杠
        if (!path2.empty() && path2.front() == '/')
        {
            combinedPath += path2.substr(1);
        }
        else
        {
            combinedPath += path2;
        }
        return combinedPath;
    }

private:
    static std::string const & getRoot()
    {
        static char const * envRoot = getenv("LOG_ROOT_PATH");
        static char const * givenRoot = (envRoot != nullptr ? envRoot : logRoot);
        static std::string root = (givenRoot != nullptr ? givenRoot : "");
        return root;
    }

    //static std::string(*foo (std::string const &)) getPathBuilder()
    static Builder getPathBuilder()
    {
        if (getRoot() != "")
            return &FileSystem::getPathRelativeRoot;
        else
            return &FileSystem::getPathRelativeBinary;
    }

    static std::string getPathRelativeRoot(const std::string& path)
    {
        return getRoot() + std::string("/") + path;
    }

    static std::string getPathRelativeBinary(const std::string& path)
    {
        return "../../../" + path;
    }

};

#endif //DEMONENGINE_FILESYSTEM_HPP
