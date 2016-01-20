
#include <iostream>
#include <algorithm>
#include "dfp_lexer.hh"
#include "dfp_program.hh"

namespace DFP {
/* Value */
bool operator==(const Value &v1, const Value &v2) {
  if (v1.type == v2.type) {
    if (v1.type == Value::IntType) {
      IntValue *iv1 = (IntValue *)&v1;
      IntValue *iv2 = (IntValue *)&v2;
      return iv1->value == iv2->value;
    } else if (v1.type == Value::StrType) {
      StrValue *sv1 = (StrValue *)&v1;
      StrValue *sv2 = (StrValue *)&v2;
      return sv1->value == sv2->value;
    }
  }
  return false;
}

/* Graph */
Node::nodetable_t Graph::getOutNodeTable() {
  Node::nodetable_t out_node_table;
  for (auto np : nt) {
    Node *n = np.second;
    if (n->type == Node::Out) {
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
      if (v->type == Value::StrType) {
        StrValue *sv = (StrValue *)v;
        string_t id = sv->value;
        if (nt[id] == NULL)
          return false;
      }
    }
  }
  return true;
}

int Graph::getBinaryNodeNumber() {
  int cnt = 0;
  for (auto np : nt)
    if (np.second->isBinary())
      cnt++;
  return cnt;
}

/* Node */

string_t Node::TypeStr[] = {"In", "Out", "Add", "Sub", "Div", "Mult", "Null"};

Node::Type Node::tag2type(Tag t) {

  switch (t) {
  case IN:
    return Node::In;
  case OUT:
    return Node::Out;
  case ADD:
    return Node::Add;
  case SUB:
    return Node::Sub;
  case MULT:
    return Node::Mult;
  case DIV:
    return Node::Div;
  default:
    return Node::Null;
  }
}

int Node::eval(int v1, int v2) {
  switch (type) {
  case Add:
    return v1 + v2;
  case Sub:
    return v1 - v2;
  case Mult:
    return v1 * v2;
  case Div:
    return v1 / v2;
  default:
    return 0;
  }
}

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
    if (src_node->type == Node::Out && dst_node->type == Node::In)
      return true;
    else
      return false;
  } else {
    return false;
  }
}

/* Program */
Program::report_t Program::getReport() {
  report_t report;
  for (auto gp : graphs) {
    string_t graph_id = gp.first;
    int num_graph_node = gp.second->getBinaryNodeNumber();
    report[graph_id] = num_graph_node;
  }
  return report;
}

// Output DFP
std::ostream &operator<<(std::ostream &os, const Value &v) {
  if (v.type == Value::IntType) {
    os << ((Num *)v.token)->value;
  } else if (v.type == Value::StrType) {
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
  os << Node::TypeStr[n.type] << "\t";
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
