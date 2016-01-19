
#ifndef DFP_H__
#define DFP_H__

namespace DFP {
class Compiler {
public:
  Compiler(std::string file_name) : file_name(file_name) {}

  int compile();

private:
  std::string file_name;
};
}

#endif
