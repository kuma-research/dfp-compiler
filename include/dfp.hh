
#ifndef DFP_H__
#define DFP_H__

#include "dfp_program.hh"

namespace DFP {

class Compiler {
public:
  Compiler(std::string file_name) : file_name(file_name) {}

  int compile();
  int optimize();
  int print(std::string out_file_name);

private:
  std::string file_name;
  Program *dfp; // compiled DFG data structure.
};
}

#endif
