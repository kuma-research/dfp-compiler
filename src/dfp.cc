
#include <iostream>
#include <string>

#include <cstdio>

#include "dfp.hh"
#include "dfp_lexer.hh"

namespace DFP {
int Compiler::compile() {
  FILE *fp = fopen(file_name.c_str(), "r");
  if (!fp) {
    fprintf(stderr, "Error: cannot open file %s.\n", file_name.c_str());
    return 1;
  }

  Lexer lexer(fp);
  lexer.scan();

  return 0;
}
}
