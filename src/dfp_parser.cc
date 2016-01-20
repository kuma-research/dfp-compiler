
#include <iostream>
#include <cstdlib>
#include "dfp_parser.hh"

namespace DFP {

void Parser::move() { look = lexer->scan(); }

void Parser::match(int t) {
  if (look->tag == t) {
    move();
  } else {
    fprintf(stderr, "Error: syntax error at %d\n", Lexer::line);
    exit(1);
  }
}

Program *Parser::program() {
  Graph *g;
  Edge *e;

  while ((g = graph()) != NULL) {
    graphs[g->id] = g; // insert a new graph
  }
  while ((e = edge()) != NULL) {
    edges.push_back(e);
  }
  return new Program(graphs, edges);
}

Graph *Parser::graph() {
  if (look->tag == DFG) {
    move();            // move to the next token
    Token *tok = look; // which should be an id
    match(ID);
    match('{'); // then followed curly braces
    nodelist_t nl = nodelist();
    match('}'); // ended curly braces
    match(';');
    Graph *g = new Graph(((Word *)tok)->lexeme, nl);
    if (nl.size() != g->nt.size()) {
      fprintf(stderr, "Fatal: same node name repeated\n");
      exit(1);
    }
    if (!g->validate()) {
      fprintf(stderr, "Fatal: unconnected nodes in graph %s\n", g->id.c_str());
      exit(1);
    }
    return g;
  } else {
    return NULL;
  }
}

nodelist_t Parser::nodelist() {
  nodelist_t nl;
  Node *n;
  while ((n = node()) != NULL) {
    nl.push_back(n);
  }
  return nl;
}

Node *Parser::node() {
  if (look->tag == IN || look->tag == OUT || look->tag == ADD ||
      look->tag == SUB || look->tag == MULT || look->tag == DIV) {
    NodeType ntype = Node::tag2type((Tag)look->tag);
    move(); // This is an InNode
    Token *tok = look;
    string_t id = ((Word *)tok)->lexeme;
    match(ID);
    match('[');
    valuelist_t vl = valuelist();

    // check value list validity
    if (ntype == In && vl.size() > 0) {
      fprintf(stderr, "Error: Input node %s should have no input value\n",
              id.c_str());
      exit(1);
    } else if (ntype == Out && vl.size() != 1) {
      fprintf(stderr, "Error: Output node %s should be unary.\n", id.c_str());
      exit(1);
    } else if (ntype != In && ntype != Out && vl.size() != 2) {
      fprintf(stderr, "Error: Binary node %s should be binary.\n", id.c_str());
      exit(1);
    }
    match(']');
    match(';');
    return new Node(ntype, id, vl);
  } else {
    return NULL;
  }
}

valuelist_t Parser::valuelist() {
  valuelist_t vl;
  Value *v;
  while ((v = value()) != NULL) {
    vl.push_back(v);
  }
  return vl;
}

Value *Parser::value() {
  if (look->tag == ID) {
    Token *tok = look;
    match(ID);
    return new StrValue((Word *)tok);
  } else if (look->tag == NUM) {
    Token *tok = look;
    match(NUM);
    return new IntValue((Num *)tok);
  } else {
    return NULL;
  }
}

Edge *Parser::edge() {
  if (look->tag == ID) {
    string_t src_graph_id = ((Word *)look)->lexeme;
    match(ID);
    match('[');
    string_t src_node_id = ((Word *)look)->lexeme;
    match(ID);
    match(']');

    match(ARROW);

    string_t dst_graph_id = ((Word *)look)->lexeme;
    match(ID);
    match('[');
    string_t dst_node_id = ((Word *)look)->lexeme;
    match(ID);
    match(']');

    Graph *src_graph = graphs[src_graph_id];
    Graph *dst_graph = graphs[dst_graph_id];
    if (src_graph != NULL && dst_graph != NULL) {
      Node *src_node = src_graph->nt[src_node_id];
      Node *dst_node = dst_graph->nt[dst_node_id];

      Edge *e = new Edge(src_graph, src_node, dst_graph, dst_node);
      if (!e->validate()) {
        fprintf(stderr, "Fatal: Edge is invalid, line %d\n", Lexer::line);
        exit(1);
      } else {
        return e;
      }
    } else {
      fprintf(stderr, "Fatal: Edge has invalid graph id, line %d\n",
              Lexer::line);
      exit(1);
    }
  } else {
    return NULL;
  }
}
}
