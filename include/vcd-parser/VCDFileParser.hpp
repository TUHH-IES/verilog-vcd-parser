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

#define YY_DECL VCDParser::parser::symbol_type yylex(VCDFileParser &driver)

YY_DECL;


/*!
@brief Class for parsing files containing CSP notation.
*/
class VCDFileParser {

public:
  //! Create a new parser/
  VCDFileParser() {
    start_time = -std::numeric_limits<decltype(start_time)>::max();
    end_time = std::numeric_limits<decltype(end_time)>::max();

    trace_scanning = false;
    trace_parsing = false;
  }
  virtual ~VCDFileParser() = default;

  /*!
  @brief Parse the suppled file.
  @returns A handle to the parsed VCDFile object or nullptr if parsing
  fails.
  */
  VCDFile *parse_file(const std::string &f) {

    filepath = f;

    scan_begin();

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

    VCDParser::parser parser(*this);

    parser.set_debug_level(trace_parsing);

    int result = parser.parse();

    scopes.pop();

    scan_end();

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
  void scan_begin();

  //! Utility function for stopping parsing.
  void scan_end();
};
