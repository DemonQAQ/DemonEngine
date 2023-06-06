cmake_minimum_required(VERSION 3.25)
project(DemonEngine)

set(CMAKE_CXX_STANDARD 20)
set(ROOT_PATH "D:/Document/program/DemonEngine")
#set(ROOT_PATH "C:/Users/Demon/CLionProjects/DemonEngine")

#add_executable(DemonEngine engine/source/editor/main.cpp engine/source/editor/interface/Interface.hpp engine/source/editor/interface/RuntimeModule.hpp engine/source/editor/interface/RuntimeModule.hpp engine/source/editor/interface/Application.hpp engine/source/editor/common/BaseApplication.hpp engine/source/editor/common/BaseApplication.cpp engine/source/editor/common/EmptyApplication.cpp engine/source/editor/common/GraphicsManager.hpp engine/source/editor/common/GraphicsManager.cpp engine/source/editor/platform/windows/DemonEngineOpenGL.cpp engine/source/editor/platform/windows/math.hpp)
#add_executable(DemonEngine engine/source/editor/platform/windows/DemonEngineOpenGL.cpp engine/source/editor/platform/windows/math.hpp engine/source/editor/openglTest/OpenGlTest.cpp)
add_executable(DemonEngine engine/source/editor/openglTest/OpenGlTest.cpp engine/source/editor/openglTest/glad.c)

INCLUDE_DIRECTORIES(${ROOT_PATH}/include)
link_directories(${ROOT_PATH}/lib)

target_link_libraries(DemonEngine ${ROOT_PATH}/lib/glfw3.dll)
target_link_libraries(DemonEngine opengl32)
