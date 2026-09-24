cmake_minimum_required(VERSION 3.20)
project(CppStudy CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # clangd 用

find_package(SDL3 REQUIRED)

# 全ターゲット共通の警告設定とサニタイザ
add_library(project_options INTERFACE)
target_compile_options(project_options INTERFACE
    -Wall -Wextra -g -fsanitize=address,undefined)
target_link_options(project_options INTERFACE -fsanitize=address,undefined)
target_include_directories(project_options INTERFACE src)

# --- 本体: SDL3 を使ったゲームループのデモ ---
add_executable(engine
    src/main.cpp
    src/window.cpp
    src/entity.cpp
    src/scene.cpp
    src/input.cpp
    src/sdl_context.cpp
)
target_link_libraries(engine PRIVATE project_options SDL3::SDL3)

# --- RAII / ムーブセマンティクスの練習用デモ（SDL 不要） ---
add_executable(raii_demo
    src/raii_demo.cpp
    src/texture.cpp
)
target_link_libraries(raii_demo PRIVATE project_options)
