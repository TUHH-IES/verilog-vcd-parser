find_package(BISON)
find_package(FLEX)
BISON_TARGET(VCDParser ${CMAKE_CURRENT_SOURCE_DIR}/vcd-parser/VCDParser.ypp ${CMAKE_CURRENT_BINARY_DIR}/VCDParser.cpp COMPILE_FLAGS -l)
FLEX_TARGET(VCDScanner ${CMAKE_CURRENT_SOURCE_DIR}/vcd-parser/VCDScanner.l  ${CMAKE_CURRENT_BINARY_DIR}/VCDScanner.cpp  COMPILE_FLAGS "--header-file=${CMAKE_CURRENT_BINARY_DIR}/VCDScanner.hpp -L")
ADD_FLEX_BISON_DEPENDENCY(VCDScanner VCDParser)


add_library(vcd-parser ${BISON_VCDParser_OUTPUTS} ${FLEX_VCDScanner_OUTPUTS})
add_library(vcd-parser::vcd-parser ALIAS vcd-parser)

if(BUILD_SHARED_LIBS AND WIN32)
  target_compile_definitions(vcd-parser
          PRIVATE "VCD_PARSER_EXPORT=__declspec(dllexport)"
          INTERFACE "VCD_PARSER_EXPORT=__declspec(dllimport)")
endif()

target_include_directories(vcd-parser PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
target_compile_features(vcd-parser PUBLIC cxx_std_17)