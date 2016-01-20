/* file: include/dfg.hh - header file for DFG data structures. */

#ifndef DFG_H__
#define DFG_H__

#include <map>
#include <vector>

#include "dfp_lexer.hh"

namespace DFP {

class Node;
class Graph;
class Edge;
class Value;

typedef std::map<string_t, Node *> nodetable_t;
typedef std::map<string_t, Graph *> graphtable_t;
typedef std::vector<Node *> nodelist_t;
typedef std::vector<Edge *> edgelist_t;
typedef std::vector<Value *> valuelist_t;

enum ValueType { IntType, StrType };
enum NodeType { In, Out, Add, Sub, Div, Mult, Null };
static std::string NodeTypeStr[] = {"In",  "Out",  "Add", "Sub",
                                    "Div", "Mult", "Null"};

class Value {
public:
  Token *token;
  ValueType type;
  Value(Token *token, ValueType type) : token(token), type(type) {}
  friend std::ostream &operator<<(std::ostream &os, const Value &v);
  friend bool operator==(const Value &v1, const Value &v2);
};

class IntValue : public Value {
public:
  int value;
  IntValue(Num *num) : Value(num, IntType), value(num->value) {}
};

class StrValue : public Value {
public:
  string_t value;
  StrValue(Word *word) : Value(word, StrType), value(word->lexeme) {}
};

class Node {
public:
  Node(NodeType type, string_t id, valuelist_t &vl)
      : type(type), id(id), vl(vl) {}

  NodeType type;
  valuelist_t vl;
  string_t id;

  bool isBinary() { return type != In && type != Out; }
  static NodeType tag2type(Tag t);
  friend std::ostream &operator<<(std::ostream &os, const Node &n);
  friend bool operator==(const Node &n1, const Node &n2);
};

class Graph {
public:
  Graph(string_t id, nodelist_t nl) : id(id) {
    for (Node *n : nl) {
      nt[n->id] = n;
    }
  }

  string_t id;
  nodetable_t nt;

  nodetable_t getOutNodeTable();
  nodetable_t getInpNodeTable();
  bool validate();
  friend std::ostream &operator<<(std::ostream &os, const Graph &g);
};

class Edge {
public:
  Edge(Graph *g1, Node *n1, Graph *g2, Node *n2)
      : src_graph(g1), src_node(n1), dst_graph(g2), dst_node(n2) {}

  bool validate();

private:
  Graph *src_graph, *dst_graph;
  Node *src_node, *dst_node;

  friend std::ostream &operator<<(std::ostream &os, const Edge &e);
};

class Program {
public:
  Program(graphtable_t &gs, edgelist_t &es) : graphs(gs), edges(es) {}

  graphtable_t graphs;
  edgelist_t edges;

  friend std::ostream &operator<<(std::ostream &os, const Program &dfg);
};
}

#endif
