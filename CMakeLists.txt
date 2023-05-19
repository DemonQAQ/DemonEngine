cmake_minimum_required(VERSION 3.25)
project(DemonEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(DemonEngine engine/source/editor/main.cpp engine/source/editor/interface/Interface.h engine/source/editor/interface/RuntimeModule.h engine/source/editor/interface/RuntimeModule.h engine/source/editor/interface/Application.h engine/source/editor/common/BaseApplication.h engine/source/editor/common/BaseApplication.cpp engine/source/editor/common/EmptyApplication.cpp)
