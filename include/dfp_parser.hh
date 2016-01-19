#ifndef DFP_PARSER_H__
#define DFP_PARSER_H__

#include "dfp_program.hh"
#include "dfp_lexer.hh"

namespace DFP {

class Parser {
public:
  Parser(Lexer *l) {
    lexer = l;
    move(); /* initial move */
  }
  // parser
  void move();
  void match(int t);

  // actions
  DFGraph *program();
  Graph *graph();
  Node *node();
  nodelist_t nodelist();
  Value *value();
  valuelist_t valuelist();
  Edge *edge();

  graphtable_t graphs;
  edgelist_t edges;

private:
  Lexer *lexer;
  Token *look;
};
}

#endif
