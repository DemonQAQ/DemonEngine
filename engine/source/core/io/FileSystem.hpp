//
// Created by Demon on 2023/7/30.
//

#ifndef DEMONENGINE_FILESYSTEM_HPP
#define DEMONENGINE_FILESYSTEM_HPP

#include <string>
#include <cstdlib>
static char const * logRoot = LOG_ROOT_PATH;
static char const * sourceRootPath = SOURCE_ROOT_PATH;

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
