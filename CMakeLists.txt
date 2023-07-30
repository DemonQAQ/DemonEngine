cmake_minimum_required(VERSION 3.25)
project(DemonEngine)

add_definitions("-DLOG_ROOT_PATH=\"${CMAKE_SOURCE_DIR}\"")

set(CMAKE_CXX_STANDARD 20)
set(ROOT_PATH "D:/Document/program/DemonEngine")

add_executable(DemonEngine engine/source/editor/openglTest/LightingTest.cpp engine/source/editor/openglTest/glad.c engine/source/editor/openglTest/base/Shader.cpp engine/source/editor/openglTest/base/Shader.hpp engine/source/editor/openglTest/base/Camera.hpp engine/source/editor/openglTest/base/FileSystem.hpp engine/source/editor/openglTest/base/root_directory.h)

include_directories(${ROOT_PATH}/include)
include_directories(${ROOT_PATH}/engine/source/editor/openglTest)

link_directories(${ROOT_PATH}/lib)

target_link_libraries(DemonEngine ${ROOT_PATH}/lib/glfw3.dll)
target_link_libraries(DemonEngine opengl32)