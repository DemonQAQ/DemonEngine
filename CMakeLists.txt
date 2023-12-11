cmake_minimum_required(VERSION 3.25)
project(DemonEngine)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_CXX_STANDARD 20)
set(ROOT_PATH "D:/Document/program/DemonEngine")

# 设置包含目录
include_directories(${ROOT_PATH}/include)
include_directories(${ROOT_PATH}/engine/source)

# 设置链接目录
link_directories(${ROOT_PATH}/lib)

# 自动将源代码文件（包括.cpp、.hpp、.c和.h文件）添加到项目中
file(GLOB_RECURSE SOURCE_FILES
        "${ROOT_PATH}/engine/source/*.cpp"
        "${ROOT_PATH}/engine/source/*.hpp"
        "${ROOT_PATH}/engine/source/*.c"
        "${ROOT_PATH}/engine/source/*.h"
        )

# 添加可执行文件
add_executable(DemonEngine ${SOURCE_FILES} engine/source/core/render/graphApi/GraphApi.hpp engine/source/core/render/graphApi/OpenglApi.hpp engine/source/core/render/pipeline/RenderPipeline.hpp engine/source/core/assets/AssetsManager.hpp engine/source/core/assets/ShaderManager.hpp)

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

# 将DLL复制到可执行文件目录
add_custom_command(TARGET DemonEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ROOT_PATH}/dlls/glfw3.dll"
        $<TARGET_FILE_DIR:DemonEngine>)