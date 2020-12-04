cmake_minimum_required(VERSION 3.10)
project(vcd-parser)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(include)

add_executable(VCDStandalone ${CMAKE_CURRENT_SOURCE_DIR}/src/VCDStandalone.cpp)
target_link_libraries(VCDStandalone vcd-parser)