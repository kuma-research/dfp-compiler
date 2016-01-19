
#include <iostream>

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <unistd.h>

#include "dfp.hh"

using namespace std;

int main(int argc, char *argv[]) {

  int c;
  int r;

  string file_name;

  while ((c = getopt(argc, argv, "f:")) != -1) {
    switch (c) {
    case 'f':
      file_name = string(optarg);
      break;
    default:
      fprintf(stderr, "Error option: %c\n", c);
      exit(1);
    }
  }

  if (file_name.size() == 0) {
    fprintf(stderr, "Error: option -f must be set.\n");
    exit(1);
  }
  DFP::Compiler compiler(file_name);
  if ((r = compiler.compile())) {
    fprintf(stderr, "Error: compile error %d\n", r);
    exit(1);
  }

  return 0;
}
