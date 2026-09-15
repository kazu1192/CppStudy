cmake_minimum_required(VERSION 3.20)
project(CppStudy CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # clangd 用

find_package(SDL3 REQUIRED)

add_executable(engine
    src/main.cpp
    src/window.cpp
    src/entity.cpp
    src/scene.cpp
    src/input.cpp
)

target_include_directories(engine PRIVATE src)
target_link_libraries(engine PRIVATE SDL3::SDL3)
target_compile_options(engine PRIVATE
    -Wall -Wextra -g -fsanitize=address,undefined)
target_link_options(engine PRIVATE -fsanitize=address,undefined)
