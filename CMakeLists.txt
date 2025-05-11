cmake_minimum_required(VERSION 3.5)

project(advent_of_code_2022 LANGUAGES CXX)

add_subdirectory(extern/raylib)

add_executable(ray_tracing_cpp 
    src/main.cpp
    src/utils.cpp
    src/camera.cpp
    src/material_lambertian.cpp
    src/material_metal.cpp
    src/material_dielectric.cpp
)

# target_include_directories(aoc_2022 PUBLIC
#     extern/inc)

target_link_libraries(ray_tracing_cpp raylib)

target_include_directories(ray_tracing_cpp PUBLIC
    extern/raylib/src)

set_property(TARGET ray_tracing_cpp PROPERTY CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS "-std=c++26 -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++")

