find_package(BISON)
find_package(FLEX)
BISON_TARGET(VCDParser ${CMAKE_CURRENT_SOURCE_DIR}/vcd-parser/VCDParser.ypp ${CMAKE_CURRENT_BINARY_DIR}/VCDParser.cpp)
FLEX_TARGET(VCDScanner ${CMAKE_CURRENT_SOURCE_DIR}/vcd-parser/VCDScanner.l  ${CMAKE_CURRENT_BINARY_DIR}/VCDLexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(VCDScanner VCDParser)


add_library(vcd-parser ${BISON_VCDParser_OUTPUTS} ${FLEX_VCDScanner_OUTPUTS})
target_include_directories(vcd-parser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})