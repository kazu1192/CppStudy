# RAII / ムーブセマンティクスの練習用デモ（SDL 不要）
add_executable(raii_demo
    raii_demo.cpp
    texture.cpp
)
target_link_libraries(raii_demo PRIVATE project_options core)
