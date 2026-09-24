add_executable(win_app main.cpp app_window.cpp)
target_link_options(win_app PRIVATE -static)
target_link_libraries(win_app PRIVATE project_options core user32 gdi32)
target_compile_definitions(win_app PRIVATE UNICODE _UNICODE WIN32_LEAN_AND_MEAN)
