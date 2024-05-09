cmake_minimum_required(VERSION 3.12)

project(Conway_Game_of_Life
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(gol gol.cpp)
target_link_libraries(gol ${SDL2_LIBRARIES})
