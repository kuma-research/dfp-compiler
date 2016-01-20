
#include <iostream>
#include <fstream>
#include <string>

#include <cstdio>

#include <queue>
#include <map>

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
  dfp = parser.program();

  std::cout << "parsed " << dfp->graphs.size() << " graphs and "
            << dfp->edges.size() << " edges" << std::endl;

  return 0;
}

int Compiler::optimize() {

  std::vector<Program::report_t> reports;

  reports.push_back(dfp->getReport());
  for (Optimizer *op : ops) {
    op->optimize(dfp);
    reports.push_back(dfp->getReport());
  }

  std::cout << "Opt Report:\tFormat=[graph #id](Binary nodes number) ..."
            << std::endl;
  for (int i = 0; i < reports.size(); i++) {
    std::cout << "Opt pass #" << i << ":\t";
    for (auto rp : reports[i]) {
      std::cout << rp.first << "(" << rp.second << ")\t";
    }
    std::cout << std::endl;
  }

  return 0;
}

int Compiler::print(std::string out_file_name) {
  std::ofstream fout(out_file_name.c_str());
  if (!fout.is_open()) {
    fprintf(stderr, "Error: cannot open output file: %s\n",
            out_file_name.c_str());
    return 1;
  }

  fout << *dfp << std::endl;
  return 0;
}
}
