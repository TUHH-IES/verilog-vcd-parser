#include <vcd-parser/VCDFileParser.hpp>
#include <vcd-parser/VCDComparisons.hpp>

#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <cctype>

inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
          }));
}

inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
          }).base(), s.end());
}

inline void trim(std::string &s) {
  rtrim(s);
  ltrim(s);
}

inline std::string trim_copy(std::string s) {
  trim(s);
  return s;
}

TEST_CASE("Basic parsing", "[VCD]") {
  VCDFileParser parser;

  auto trace = parser.parse_file("../../tests/testfiles/simple.vcd");
  REQUIRE(trace != nullptr);
  CHECK(trim_copy(trace->version) == "Icarus Verilog");
  CHECK(trim_copy(trace->date) == "Wed Mar 01 17:08:44 2023");
  CHECK(trace->get_signals().size() == 14);
  CHECK(trace->get_timestamps().size() == 11);
}

TEST_CASE("Advanced parsing", "[VCD]") {
    VCDFileParser parser;

    auto trace = parser.parse_file("../../tests/testfiles/advanced.vcd");
    REQUIRE(trace != nullptr);
    CHECK(trim_copy(trace->version) == "Icarus Verilog");
    CHECK(trim_copy(trace->date) == "Fri Oct  6 16:38:36 2023");
    CHECK(trace->get_signals().size() == 3354);
    CHECK(trace->get_timestamps().size() == 2201);
}

TEST_CASE("Basic compare", "[VCD]") {
  VCDFileParser parser;

  auto trace1 = parser.parse_file("../../tests/testfiles/simple.vcd");
  REQUIRE(trace1 != nullptr);

  auto trace2 = parser.parse_file("../../tests/testfiles/simple.vcd");
  REQUIRE(trace2 != nullptr);

  CHECK(*trace1 == *trace2);
}