# SDL3 を使ったゲームループのデモ（本体）
find_package(SDL3 REQUIRED)

add_executable(engine
    main.cpp
    window.cpp
    entity.cpp
    scene.cpp
    input.cpp
    sdl_context.cpp
)
target_link_libraries(engine PRIVATE project_options core SDL3::SDL3)
