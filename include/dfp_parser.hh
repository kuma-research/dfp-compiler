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
  Program *program();
  Graph *graph();
  Node *node();
  Node::nodelist_t nodelist();
  Value *value();
  Value::valuelist_t valuelist();
  Edge *edge();

  Graph::graphtable_t graphs;
  Edge::edgelist_t edges;

private:
  Lexer *lexer;
  Token *look;
};
}

#endif
