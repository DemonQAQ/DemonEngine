//
// Created by Demon on 2024/5/5.
//

#ifndef DEMONENGINE_MONOASSEMBLYCOMPILETHREAD_HPP
#define DEMONENGINE_MONOASSEMBLYCOMPILETHREAD_HPP

#include "core/base/interface/Interface.hpp"
#include "MonoThread.hpp"
#include "core/script/entity/AssemblyScriptEntity.hpp"
#include <core/base/lib/BoolDefine.hpp>
#include <mono/metadata/appdomain.h>

namespace script
{
    class MonoAssemblyCompileThread : implements MonoThread
    {
    public:
        MonoAssemblyCompileThread(const std::string &domainName_, const std::string &threadName,
                                  const std::shared_ptr<io::XmlConfiguration> &config_ = nullptr) :
                MonoThread(domainName_, threadName, config_)
        {

        }

        std::shared_ptr<AssemblyScriptEntity>
        compileCSharpFile(std::shared_ptr<base::UUID> &existingUuid, std::string scriptPath,
                          std::string name, std::vector<char> &scriptData)
        {
            std::shared_ptr<AssemblyScriptEntity> resultEntity;
            std::mutex resultMutex;
            std::condition_variable resultCondition;

            auto task = [&]()
            {
                std::cerr << "start compiler " << std::endl;
                if (scriptPath.size() >= 3 && scriptPath.substr(scriptPath.size() - 3) == ".cs")
                {
                    std::cerr << ".cs compiler start" << std::endl;
                    // 编译.cs文件到.dll
                    std::string assemblyPath = scriptPath.substr(0, scriptPath.size() - 3) + ".dll";
                    std::string compileCommand = "mcs /target:library -out:" + assemblyPath + " " + scriptPath;
                    system(compileCommand.c_str());

                    // 加载编译后的程序集
                    MonoAssembly *assembly = mono_domain_assembly_open(domain, assemblyPath.c_str());
                    if (!assembly)
                    {
                        std::cerr << "Failed to load the compiled assembly: " << assemblyPath << std::endl;
                        return;
                    }

                    MonoImage *image = mono_assembly_get_image(assembly);
                    if (!image)
                    {
                        std::cerr << "Failed to get image from assembly: " << assemblyPath << std::endl;
                        return;
                    }
                    resultEntity = std::make_shared<AssemblyScriptEntity>(existingUuid, name, assembly, image);
                    std::cerr << ".cs compiler end" << std::endl;
                }
                else
                {
                    std::cerr << "dll/exe compiler start" << std::endl;
                    MonoImageOpenStatus status;
                    MonoImage *image = mono_image_open_from_data_with_name(scriptData.data(), scriptData.size(), TRUE,
                                                                           &status, FALSE, scriptPath.c_str());
                    if (status != MONO_IMAGE_OK || image == nullptr)
                    {
                        std::cerr << "Failed to load script image: " << scriptPath << std::endl;
                        return;
                    }
                    MonoAssembly *assembly = mono_assembly_load_from(image, scriptPath.c_str(), &status);
                    mono_image_close(image);

                    if (assembly)
                    {
                        resultEntity = std::make_shared<AssemblyScriptEntity>(existingUuid, name, assembly, image);
                    }
                    std::cerr << "dll/exe compiler end" << std::endl;
                }
                std::cerr << "end compiler " << std::endl;
            };

            submitTask(task);

            {
                std::unique_lock<std::mutex> lock(resultMutex);
                if (!resultCondition.wait_for(lock, std::chrono::seconds(10), [&]
                { return resultEntity != nullptr; }))
                {
                    std::cerr << "Timeout waiting for task completion." << std::endl;
                    return nullptr;
                }
            }

            return resultEntity;
        }
    };
}

#endif //DEMONENGINE_MONOASSEMBLYCOMPILETHREAD_HPP
