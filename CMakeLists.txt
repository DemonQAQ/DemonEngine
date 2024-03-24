cmake_minimum_required(VERSION 3.25)
project(DemonEngine)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_CXX_STANDARD 20)
set(ROOT_PATH "D:/Document/program/DemonEngine")

# 设置包含目录
include_directories(${ROOT_PATH}/include)
include_directories(${ROOT_PATH}/engine/source)
set(CMAKE_PREFIX_PATH D:/developLib/boost)

# 设置链接目录
link_directories(${ROOT_PATH}/lib)

find_package(Boost REQUIRED COMPONENTS filesystem)

# 自动将源代码文件（包括.cpp、.hpp、.c和.h文件）添加到项目中
file(GLOB_RECURSE SOURCE_FILES
        "${ROOT_PATH}/engine/source/*.cpp"
        "${ROOT_PATH}/engine/source/*.hpp"
        "${ROOT_PATH}/engine/source/*.c"
        "${ROOT_PATH}/engine/source/*.h"
        )

# 添加可执行文件
add_executable(DemonEngine ${SOURCE_FILES} engine/source/core/render/graphApi/GraphApi.hpp engine/source/core/render/graphApi/OpenGLApi.hpp engine/source/core/render/pipeline/RenderPipeline.hpp engine/source/core/assets/AssetsMainManager.hpp engine/source/core/assets/ShaderManager.hpp engine/source/core/render/pipeline/DrawCall.hpp engine/source/core/base/render/Mesh.hpp engine/source/core/base/render/Model.hpp engine/source/core/base/render/Texture.hpp engine/source/core/base/render/Material.hpp engine/source/core/base/render/Vertex.hpp engine/source/core/base/render/RenderableObject.hpp engine/source/core/base/utils/UUIDUtil.hpp engine/source/core/base/common/UUID.hpp engine/source/core/base/common/Object.hpp engine/source/core/base/render/Bone.hpp engine/source/core/base/interface/ITransformable.hpp engine/source/core/base/common/Transform.hpp engine/source/core/base/interface/IRenderable.hpp engine/source/core/base/interface/ITransformableUpdate.hpp engine/source/core/base/interface/INameable.hpp engine/source/core/assets/interface/IFileManager.hpp engine/source/core/assets/MediaManager.hpp engine/source/core/assets/TextureManager.hpp engine/source/core/assets/ModelManager.hpp engine/source/core/assets/MaterialsManager.hpp engine/source/core/assets/AssetType.hpp engine/source/core/assets/ConfigManager.hpp engine/source/core/base/render/Mesh.cpp engine/source/core/base/render/Model.cpp engine/source/core/assets/ModelManager.cpp engine/source/core/assets/ModelManager.cpp engine/source/core/assets/TextureManager.cpp engine/source/core/assets/ShaderManager.cpp engine/source/core/assets/MaterialsManager.cpp engine/source/core/assets/ConfigManager.cpp engine/source/core/io/interface/IConfigurable.hpp engine/source/core/io/config/YamlConfiguration.cpp engine/source/core/io/config/YamlConfiguration.hpp engine/source/core/base/lib/stb_image.cpp
        engine/source/core/io/config/JsonConfiguration.cpp
        engine/source/core/io/config/JsonConfiguration.hpp
        engine/source/core/io/config/XmlConfiguration.cpp
        engine/source/core/io/config/XmlConfiguration.hpp
        engine/source/core/io/interface/IConfigurationSection.hpp
        engine/source/core/assets/AssetsMainManager.cpp
        engine/source/core/io/FileType.hpp
        engine/source/core/render/graphApi/OpenGLApi.cpp
        engine/source/core/base/render/RenderableObject.cpp
        engine/source/core/base/render/Light.cpp
        engine/source/core/base/render/Light.hpp
        engine/source/core/render/pipeline/OpenglDrawCall.cpp
        engine/source/core/render/pipeline/OpenglDrawCall.hpp
        engine/source/core/render/pipeline/RenderManager.hpp
        engine/source/core/render/manager/Initializer.hpp
        engine/source/core/base/common/platform/OpenglApplication.hpp
)

# 添加编译选项
if (MSVC)
    add_compile_options(/utf-8)
endif ()

# 添加宏定义
add_definitions("-DLOG_ROOT_PATH=\"${CMAKE_SOURCE_DIR}\"")

# 链接库
target_link_libraries(DemonEngine ${ROOT_PATH}/lib/glfw3.lib)
target_link_libraries(DemonEngine opengl32)
target_link_libraries(DemonEngine ${ROOT_PATH}/lib/zlibstaticd.lib)
target_link_libraries(DemonEngine ${ROOT_PATH}/lib/assimp-vc143-mtd.lib)
target_link_libraries(DemonEngine ${ROOT_PATH}/lib/yaml-cpp.lib)
target_link_libraries(${PROJECT_NAME} Boost::filesystem)

# 将DLL复制到可执行文件目录
add_custom_command(TARGET DemonEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ROOT_PATH}/dlls/glfw3.dll"
        $<TARGET_FILE_DIR:DemonEngine>)