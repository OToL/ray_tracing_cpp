project(advent_of_code_2022 LANGUAGES CXX)

add_executable(ray_tracing_cpp 
    src/main.cpp)

# target_include_directories(aoc_2022 PUBLIC
#     extern/inc)

set_property(TARGET ray_tracing_cpp PROPERTY CXX_STANDARD 26)
set(CMAKE_CXX_FLAGS "-std=c++26 -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "-stdlib=libc++")


