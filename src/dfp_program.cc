
#include <iostream>
#include <algorithm>
#include "dfp_lexer.hh"
#include "dfp_program.hh"

namespace DFP {
NodeType Node::tag2type(Tag t) {

  switch (t) {
  case IN:
    return In;
  case OUT:
    return Out;
  case ADD:
    return Add;
  case SUB:
    return Sub;
  case MULT:
    return Mult;
  case DIV:
    return Div;
  default:
    return Null;
  }
}

std::ostream &operator<<(std::ostream &os, const Value &v) {
  if (v.type == IntType) {
    os << ((Num *)v.token)->value;
  } else if (v.type == StrType) {
    os << ((Word *)v.token)->lexeme;
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const IntValue &v) {
  os << v.value;
  return os;
};

std::ostream &operator<<(std::ostream &os, const StrValue &v) {
  os << v.value;
  return os;
};

std::ostream &operator<<(std::ostream &os, const Node &n) {
  os << NodeTypeStr[n.type] << "\t";
  os << n.id << "\t[ ";
  for (Value *v : n.vl) {
    os << *v << " ";
  }
  os << "];";
  return os;
}

std::ostream &operator<<(std::ostream &os, const Graph &g) {
  os << "dfg " << g.id << "\t{" << std::endl;
  for (auto np : g.nt) {
    os << "  " << *(np.second) << std::endl;
  }
  os << "};" << std::endl;
  return os;
}

// src_graph_id[src_node_id]->dst_graph_id[dst_node_id]
std::ostream &operator<<(std::ostream &os, const Edge &e) {
  os << e.src_graph->id << "[" << e.src_node->id << "]";
  os << "\t-> ";
  os << e.dst_graph->id << "[" << e.dst_node->id << "]";
  os << ";";
  return os;
};
std::ostream &operator<<(std::ostream &os, const DFGraph &dfg) {
  for (auto ng : dfg.graphs) {
    os << *(ng.second) << std::endl;
  }

  for (Edge *e : dfg.edges) {
    os << *e << std::endl;
  }
  return os;
}
}
