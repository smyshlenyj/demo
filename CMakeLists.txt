cmake_minimum_required(VERSION 3.16)
project(calc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(lib)
add_executable(calc main.cpp)

target_link_libraries(calc PRIVATE math)