cmake_minimum_required(VERSION 3.20)
project(CppStudy CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)   # clangd 用

# サブディレクトリに分けても実行ファイルは build/ 直下に出す
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# 全ターゲット共通の警告設定とサニタイザ
add_library(project_options INTERFACE)
target_compile_options(project_options INTERFACE -Wall -Wextra -g)
if(NOT WIN32)   # mingw-w64 の GCC は ASan/UBSan 非対応
    target_compile_options(project_options INTERFACE -fsanitize=address,undefined)
    target_link_options(project_options INTERFACE -fsanitize=address,undefined)
endif()

# OS 非依存の共有ヘッダ（Vec2 / Vec3 / AABB / Color）
add_library(core INTERFACE)
target_include_directories(core INTERFACE core)

if(NOT WIN32)
    add_subdirectory(app/sdl_engine)
    add_subdirectory(app/raii_demo)
endif()
if(WIN32)
    # まだ空のうちは configure を失敗させない
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/app/win32gui/CMakeLists.txt)
        add_subdirectory(app/win32gui)
    else()
        message(STATUS "app/win32gui はまだ空です（Windows GUI アプリ未作成）")
    endif()
endif()

enable_testing()
add_subdirectory(tests)
