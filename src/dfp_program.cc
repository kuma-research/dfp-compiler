
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

/* Value */
bool operator==(const Value &v1, const Value &v2) {
  if (v1.type == v2.type) {
    if (v1.type == IntType) {
      IntValue *iv1 = (IntValue *)&v1;
      IntValue *iv2 = (IntValue *)&v2;
      return iv1->value == iv2->value;
    } else if (v1.type == StrType) {
      StrValue *sv1 = (StrValue *)&v1;
      StrValue *sv2 = (StrValue *)&v2;
      return sv1->value == sv2->value;
    }
  }
  return false;
}

/* Graph */
nodetable_t Graph::getOutNodeTable() {
  nodetable_t out_node_table;
  for (auto np : nt) {
    Node *n = np.second;
    if (n->type == Out) {
      out_node_table[n->id] = n;
    }
  }
  return out_node_table;
}

bool Graph::validate() {
  for (auto np : nt) {
    Node *n = np.second;
    // for each value in node's value list, check whether it's in the graph
    for (Value *v : n->vl) {
      if (v->type == StrType) {
        StrValue *sv = (StrValue *)v;
        string_t id = sv->value;
        if (nt[id] == NULL)
          return false;
      }
    }
  }
  return true;
}

/* Node */
bool operator==(const Node &n1, const Node &n2) {
  if (n1.type == n2.type) {
    Value *v10 = n1.vl[0];
    Value *v11 = n1.vl[1];
    Value *v20 = n2.vl[0];
    Value *v21 = n2.vl[1];

    return ((*v10 == *v20 && *v11 == *v21) || (*v11 == *v20 && *v10 == *v21));
  } else {
    return false;
  }
}

/* Edge */
bool Edge::validate() {
  if (src_graph != NULL && src_node != NULL && dst_graph != NULL &&
      dst_node != NULL) {
    if (src_node->type == Out && dst_node->type == In)
      return true;
    else
      return false;
  } else {
    return false;
  }
}

// Output DFP
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
  os << "dfg \t" << g.id << "\t{" << std::endl;
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
std::ostream &operator<<(std::ostream &os, const Program &dfg) {
  for (auto ng : dfg.graphs) {
    os << *(ng.second) << std::endl;
  }

  for (Edge *e : dfg.edges) {
    os << *e << std::endl;
  }
  return os;
}
}
