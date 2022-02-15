cmake_minimum_required(VERSION 3.10)
project(vcd-parser)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion")

add_subdirectory(include)

add_executable(vcd-demonstrator ${CMAKE_CURRENT_SOURCE_DIR}/src/VCDStandalone.cpp)
target_link_libraries(vcd-demonstrator vcd-parser)