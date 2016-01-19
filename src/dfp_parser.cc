
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

DFGraph *Parser::program() {
  Graph *g;
  Edge *e;

  while ((g = graph()) != NULL) {
    graphs[g->id] = g; // insert a new graph
  }
  while ((e = edge()) != NULL) {
    edges.push_back(e);
  }
  return new DFGraph(graphs, edges);
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
    match(ID);
    match('[');
    valuelist_t vl = valuelist();
    match(']');
    match(';');
    return new Node(ntype, ((Word *)tok)->lexeme, vl);
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
    Node *src_node = src_graph->nt[src_node_id];
    Node *dst_node = dst_graph->nt[dst_node_id];

    return new Edge(src_graph, src_node, dst_graph, dst_node);
  } else {
    return NULL;
  }
}
}
