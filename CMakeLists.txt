cmake_minimum_required(VERSION 3.13)
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "libft/*.c")
file(GLOB_RECURSE HEADERS RELATIVE ${CMAKE_SOURCE_DIR} "libft/includes/*.h")
file(GLOB_RECURSE SOURCES2 RELATIVE ${CMAKE_SOURCE_DIR} "src/main.c")
file(GLOB_RECURSE HEADERS2 RELATIVE ${CMAKE_SOURCE_DIR} "includes/*.h")
project(Fractol C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-Wall -Werror -Wextra)

include_directories("includes")
include_directories("libft/includes")
link_directories(/usr/local/lib)
link_libraries(mlx)
find_library(OPEN_GL OpenGL)
find_library(APP_KIT AppKit)

add_executable(Fract_ol ${SOURCES} ${HEADERS} ${HEADERS2} ${SOURCES2})
target_link_libraries(Fract_ol ${OPEN_GL} ${APP_KIT})