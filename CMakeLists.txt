cmake_minimum_required(VERSION 3.26)
set(VCPKG_TARGET_TRIPLET x64-windows-static-md CACHE STRING "target triplet" FORCE)
set(VCPKG_CRT_LINKAGE "static" FORCE)

project(RS3ELauncher)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

set(CMAKE_CXX_STANDARD 23)

include($ENV{VCPKG})

file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "src/*.h")
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

add_executable(RS3ELauncher ${SOURCES} ${HEADERS})

find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)
find_package(Freetype REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(plog CONFIG REQUIRED)

target_include_directories(RS3ELauncher PRIVATE ${OPENGL_INCLUDE_DIR})
target_include_directories(RS3ELauncher PRIVATE ${IMGUI_INCLUDE_DIR})

target_link_libraries(RS3ELauncher PRIVATE plog::plog)
target_link_libraries(RS3ELauncher PRIVATE GLEW::GLEW)
target_link_libraries(RS3ELauncher PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries(RS3ELauncher PUBLIC glfw)
target_link_libraries(RS3ELauncher PRIVATE imgui::imgui)
target_link_libraries(RS3ELauncher PRIVATE Freetype::Freetype)
target_link_libraries(RS3ELauncher PRIVATE CURL::libcurl)