/*!
@file
@brief Contains the declaration of the parser driver class.
*/

#pragma once

#include <vcd-parser/VCDFile.hpp>
#include <vcd-parser/VCDTypes.hpp>

#include <VCDParser.hpp>

#include <map>
#include <set>
#include <stack>
#include <string>
#include <limits>

#define YY_DECL VCDParser::parser::symbol_type yylex([[maybe_unused]] VCDFileParser &driver, yyscan_t yyscanner)
YY_DECL;

/*!
@brief Class for parsing files containing CSP notation.
*/
class VCDFileParser {

public:
  //! Create a new parser/
  explicit VCDFileParser(bool debug = false) {
    start_time = -std::numeric_limits<decltype(start_time)>::max();
    end_time = std::numeric_limits<decltype(end_time)>::max();

    trace_scanning = debug;
    trace_parsing = debug;
  }
  virtual ~VCDFileParser() = default;

  /*!
  @brief Parse the suppled file.
  @returns A handle to the parsed VCDFile object or nullptr if parsing
  fails.
  */
  VCDFile *parse_file(const std::string &f) {

    filepath = f;

    yyscan_t scanner = scan_begin();

    fh = new VCDFile();
    VCDFile *tr = this->fh;

    fh->root_scope = new VCDScope;
    fh->root_scope->name = std::string("$root");
    fh->root_scope->type = VCD_SCOPE_ROOT;

    scopes.push(this->fh->root_scope);

    fh->root_scope = new VCDScope;
    fh->root_scope->name = std::string("");
    fh->root_scope->type = VCD_SCOPE_ROOT;

    scopes.push(this->fh->root_scope);

    tr->add_scope(scopes.top());

    VCDParser::parser parser(*this, scanner);

    parser.set_debug_level(trace_parsing);

    int result = parser.parse();

    scopes.pop();

    scan_end(scanner);

    if (result == 0)
    {
      this->fh = nullptr;
      return tr;
    }
    else
    {
      tr = nullptr;
      delete this->fh;
      return nullptr;
    }
  }

  //! The current file being parsed.
  std::string filepath;

  //! Should we debug tokenising?
  bool trace_scanning;

  //! Should we debug parsing of tokens?
  bool trace_parsing;

  //! Ignore anything before this timepoint
  VCDTime start_time;

  //! Ignore anything after this timepoint
  VCDTime end_time;

  //! Reports errors to stderr.
  void error(const VCDParser::location& l, const std::string& m) {
    std::cerr << "line " << l.begin.line << std::endl;
    std::cerr << " : " << m << std::endl;
  }

  //! Reports errors to stderr.
  void error(const std::string& m) {
    std::cerr << " : " << m << std::endl;
  }

  //! Current file being parsed and constructed.
  VCDFile* fh;

  //! Current stack of scopes being parsed.
  std::stack<VCDScope*> scopes;

protected:
  //! Utility function for starting parsing.
  yyscan_t scan_begin();

  //! Utility function for stopping parsing.
  void scan_end(yyscan_t scanner);
};