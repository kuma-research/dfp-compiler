
#include <iostream>
#include <fstream>
#include <string>

#include <cstdio>

#include "dfp.hh"
#include "dfp_lexer.hh"
#include "dfp_program.hh"
#include "dfp_parser.hh"

namespace DFP {
int Compiler::compile() {
  FILE *fp = fopen(file_name.c_str(), "r");
  if (!fp) {
    fprintf(stderr, "Error: cannot open file %s.\n", file_name.c_str());
    return 1;
  }

  Parser parser(new Lexer(fp));
  dfg = parser.program();

  std::cout << "parsed " << dfg->graphs.size() << " graphs and "
            << dfg->edges.size() << " edges" << std::endl;

  return 0;
}

int Compiler::print(std::string out_file_name) {
  std::ofstream fout(out_file_name.c_str());
  if (!fout.is_open()) {
    fprintf(stderr, "Error: cannot open output file: %s\n",
            out_file_name.c_str());
    return 1;
  }

  fout << *dfg << std::endl;
  return 0;
}
}
