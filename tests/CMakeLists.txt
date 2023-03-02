Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.3.2
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain vcd-parser)

set(LIST_OF_SANITIZERS "leak,address,undefined")
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
  target_compile_options(tests INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
  target_link_libraries(tests INTERFACE -fsanitize=${LIST_OF_SANITIZERS})
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options(tests INTERFACE /fsanitize=${LIST_OF_SANITIZERS})
  target_link_libraries(tests INTERFACE /fsanitize=${LIST_OF_SANITIZERS})
endif()

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(tests)