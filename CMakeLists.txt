
cmake_minimum_required(VERSION 3.15)
project(menu_demo C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(menu_demo
    app.c
    tui_menu.c
    tui_win32.c
)

if(WIN32)
    target_compile_definitions(menu_demo PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# A header fájlok a projekt gyökerében vannak
