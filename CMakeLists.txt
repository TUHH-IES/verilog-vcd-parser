cmake_minimum_required(VERSION 3.15)
set(vcd-parser_VERSION 0.8.0)
project(vcd-parser
        VERSION ${vcd-parser_VERSION}
        DESCRIPTION "A simple VCD parser easily usable in CMake projects. This library is a fork of the library at https://github.com/ben-marshall/verilog-vcd-parser"
        HOMEPAGE_URL "https://github.com/gian21391/verilog-vcd-parser"
        LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_subdirectory(include)

add_executable(vcd-demonstrator ${CMAKE_CURRENT_SOURCE_DIR}/src/VCDStandalone.cpp)
target_link_libraries(vcd-demonstrator vcd-parser)

configure_package_config_file(cmake/config.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/vcd-parser-config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/vcd-parser
        NO_SET_AND_CHECK_MACRO)
write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/vcd-parser-config-version.cmake
        VERSION ${vcd-parser_VERSION}
        COMPATIBILITY SameMajorVersion)
install(FILES
          ${CMAKE_CURRENT_BINARY_DIR}/vcd-parser-config.cmake
          ${CMAKE_CURRENT_BINARY_DIR}/vcd-parser-config-version.cmake
        DESTINATION
          ${CMAKE_INSTALL_DATADIR}/vcd-parser)

install(TARGETS vcd-parser EXPORT vcd-parser-targets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT vcd-parser-targets
        NAMESPACE vcd-parser::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/vcd-parser)
