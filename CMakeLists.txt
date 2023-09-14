cmake_minimum_required(VERSION 3.25)
set(CMAKE_VERBOSE_MAKEFILE TRUE)
if(MSVC)
    add_compile_options(/utf-8)
endif()
project(DemonEngine)

add_definitions("-DLOG_ROOT_PATH=\"${CMAKE_SOURCE_DIR}\"")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(ROOT_PATH "D:/Document/program/DemonEngine")

add_executable(DemonEngine engine/source/editor/openglTest/glad.c engine/source/editor/openglTest/base/Shader.cpp engine/source/editor/openglTest/base/Shader.hpp engine/source/editor/openglTest/base/Camera.hpp engine/source/editor/openglTest/base/FileSystem.hpp engine/source/editor/openglTest/base/root_directory.h engine/source/editor/openglTest/base/Model.hpp engine/source/editor/openglTest/base/Mesh.hpp engine/source/editor/openglTest/ModelTest.cpp)

include_directories(${ROOT_PATH}/include)
include_directories(${ROOT_PATH}/engine/source/editor/openglTest)

link_directories(${ROOT_PATH}/lib)

target_link_libraries(DemonEngine ${ROOT_PATH}/lib/glfw3.lib)
target_link_libraries(DemonEngine opengl32)
target_link_libraries(DemonEngine ${ROOT_PATH}/lib/zlibstaticd.lib)
target_link_libraries(DemonEngine ${ROOT_PATH}/lib/assimp-vc143-mtd.lib)

add_custom_command(TARGET DemonEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ROOT_PATH}/dlls/glfw3.dll"
        $<TARGET_FILE_DIR:DemonEngine>)