cmake_minimum_required(VERSION 3.24)
set(vcd-parser_VERSION 0.8.0)
project(vcd-parser
        VERSION ${vcd-parser_VERSION}
        DESCRIPTION "A simple VCD parser easily usable in CMake projects. This library is a fork of the library at https://github.com/ben-marshall/verilog-vcd-parser"
        HOMEPAGE_URL "https://github.com/gian21391/verilog-vcd-parser"
        LANGUAGES CXX)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)
include(FetchContent)

if(WIN32)
  FetchContent_Declare(win_flex_bison
          URL https://github.com/lexxmark/winflexbison/releases/download/v2.5.25/win_flex_bison-2.5.25.zip
          URL_MD5 720226b1befe7033fb3ecc98f5ffd425
          DOWNLOAD_EXTRACT_TIMESTAMP TRUE
          )
  FetchContent_MakeAvailable(win_flex_bison)
  LIST(APPEND CMAKE_PROGRAM_PATH "${win_flex_bison_SOURCE_DIR}")
endif()

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
        DESTINATION ${CMAKE_INSTALL_DATADIR}/vcd-parser)
install(TARGETS vcd-parser EXPORT vcd-parser-targets)
install(DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
        PATTERN "*.hpp"
        PATTERN "*.hh"
        PATTERN "CMakeFiles" EXCLUDE)
install(EXPORT vcd-parser-targets
        NAMESPACE vcd-parser::
        DESTINATION ${CMAKE_INSTALL_DATADIR}/vcd-parser)
