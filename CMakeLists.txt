cmake_minimum_required(VERSION 3.25)
project(DemonEngine)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ROOT_PATH "D:/Document/program/DemonEngine")
set(MONO_PATH "C:/Program Files/Mono/lib")
set(CMAKE_PREFIX_PATH D:/developLib/boost)

# 设置包含目录
include_directories(${ROOT_PATH}/include)
include_directories(${ROOT_PATH}/engine/source)

# 设置链接目录
link_directories(${ROOT_PATH}/lib)
link_directories(MONO_PATH)

find_package(Boost REQUIRED COMPONENTS filesystem)

# 自动将源代码文件（包括.cpp、.hpp、.c和.h文件）添加到项目中
file(GLOB_RECURSE SOURCE_FILES
        "${ROOT_PATH}/engine/source/*.cpp"
        "${ROOT_PATH}/engine/source/*.hpp"
        "${ROOT_PATH}/engine/source/*.c"
        "${ROOT_PATH}/engine/source/*.h"
        )

# 添加可执行文件
add_executable(DemonEngine ${SOURCE_FILES})

# 启用预编译头
target_precompile_headers(DemonEngine PRIVATE engine/source/core/base/lib/pch.hpp)

# 添加编译选项
if (MSVC)
    add_compile_options(/utf-8)
endif ()

# 添加宏定义
target_compile_definitions(DemonEngine PRIVATE LOG_ROOT_PATH="${CMAKE_SOURCE_DIR}")
target_compile_definitions(DemonEngine PRIVATE SOURCE_ROOT_PATH="${ROOT_PATH}/assets")

# 链接库
target_link_libraries(${PROJECT_NAME} opengl32)
target_link_libraries(${PROJECT_NAME} Boost::filesystem)
target_link_libraries(DemonEngine "${MONO_PATH}/mono-2.0-sgen.lib")

# 链接所有在lib目录下的.lib文件
file(GLOB LIBRARY_FILES "${ROOT_PATH}/lib/*.lib")
foreach(LIB_FILE ${LIBRARY_FILES})
    target_link_libraries(DemonEngine "${LIB_FILE}")
endforeach()

# 将lib文件复制到输出目录
file(GLOB LIB_FILES "${ROOT_PATH}/lib/*.lib")
foreach(LIB_FILE ${LIB_FILES})
    add_custom_command(TARGET DemonEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${LIB_FILE}"
            $<TARGET_FILE_DIR:DemonEngine>)
endforeach()

# 将DLL文件复制到输出目录
file(GLOB_RECURSE DLL_FILES "${ROOT_PATH}/lib/*.dll")
foreach(DLL_FILE ${DLL_FILES})
    add_custom_command(TARGET DemonEngine POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL_FILE}"
            $<TARGET_FILE_DIR:DemonEngine>)
endforeach()