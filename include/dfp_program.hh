/* file: include/dfg.hh - header file for DFG data structures. */

#ifndef DFG_H__
#define DFG_H__

#include <map>
#include <vector>

#include "dfp_lexer.hh"

namespace DFP {

//! Value stores the elements in each Node's input list
/*! This class has design defects, it should be better with templates */
class Value {
public:
  //! An Enum for Value object type
  enum Type {
    IntType, /*!< This value is an integer */
    StrType  /*!< This value is a string */
  };

  //! A list of pointers to Value objects
  typedef std::vector<Value *> valuelist_t;

  Token *token; /*!< Token for this value, could be Int or String */
  Type type;    /*!< Type for this Value, could be IntType or StrType */

  //! Constructor
  /*! Takes a token and assign the type */
  Value(Token *token, Type type) : token(token), type(type) {}

  friend std::ostream &operator<<(std::ostream &os, const Value &v);
  friend bool operator==(const Value &v1, const Value &v2);
};

//! Inherited from Value, to store integer value.
class IntValue : public Value {
public:
  int value;
  IntValue(Num *num) : Value(num, IntType), value(num->value) {}

  //! Factory method
  static IntValue *create(int v) { return new IntValue(new Num(v)); }
};

//! Inherited from Value, to store string value.
class StrValue : public Value {
public:
  string_t value;
  StrValue(Word *word) : Value(word, StrType), value(word->lexeme) {}

  //! Factory method
  static StrValue *create(string_t s) {
    return new StrValue(new Word(s.c_str(), ID));
  }
};

//! Node contains a list of input Values, will be referened in Graph or Edge.
class Node {
public:
  //! An Enum for Node object type.
  /*! Types can be divided into 2 categories: Input/Output and Binary */
  enum Type {
    In,   /*!< This Node is an input node, has empty value list. */
    Out,  /*!< This Node is an output node, has 1 value in the value list */
    Add,  /*!< This Node will add up two input values */
    Sub,  /*!< This Node will substract two input values as [0] - [1] */
    Div,  /*!< This Node will multiply two input values */
    Mult, /*!< This Node will divide two input values as [0] / [1]*/
    Null  /*!< This Node can't be recognized */
  };
  //! Names for Node::Type
  static std::string TypeStr[];

  //! A hash table from Node id string to Node object pointer */
  typedef std::map<string_t, Node *> nodetable_t;
  //! A list of pointers to Node objects
  typedef std::vector<Node *> nodelist_t;

  //! Constructor
  /*!
    Specify the type of the node, the id, and value list for a node.
    \param type The type of this constructing node.
    \param id   The id for this node.
    \param valuelist a list of values parsed before.
  */
  Node(Type type, string_t id, Value::valuelist_t &vl)
      : type(type), id(id), vl(vl) {}

  Type type;             /*! The Type of this Node */
  Value::valuelist_t vl; /*! The input value list for this Node */
  string_t id;           /*! The id of this node */

  //! Check whether this Node object is a binary one.
  bool isBinary() { return type != In && type != Out; }

  //! Check whether this Node object is a unary one.
  bool isUnary() { return type == Out; }

  //! Use Node's type to calculate two input. NO TYPE CHECKING HERE!
  int eval(int v1, int v2);

  //! Convert Tag in Token to Type in Node.
  static Type tag2type(Tag t);
  friend std::ostream &operator<<(std::ostream &os, const Node &n);
  friend bool operator==(const Node &n1, const Node &n2);
};

//! Graph contains a hash table of nodes.
/*! Each graph contains many Nodes, all the pointers to them will be stored in a
 * hash table, which could be refereced by Node's id */
class Graph {
public:
  //! Constructor
  /*! After the parser has parsed the Node list within each Graph block, will
   * create a new Graph object by using this collection of Node. The original
   * list will be converted to a hash table.
   */
  Graph(string_t id, Node::nodelist_t nl) : id(id) {
    for (Node *n : nl) {
      nt[n->id] = n;
    }
  }
  //! A hashtable from graph id to Graph object pointer
  typedef std::map<string_t, Graph *> graphtable_t;

  string_t id;          /*!< The id of this Graph */
  Node::nodetable_t nt; /*!< The hash table in this Graph */

  //! Get the Nodes in this Graph which has type Node::Out
  Node::nodetable_t getOutNodeTable();

  //! Get the Nodes in this Graph which has type Node::In
  Node::nodetable_t getInpNodeTable();

  //! Get number of binary nodes.
  int getBinaryNodeNumber();
  bool validate();
  friend std::ostream &operator<<(std::ostream &os, const Graph &g);
};

//! Edge contains pointers to graphs and nodes.
/*! Edge contains 4 pointers: to the source graph and source node, to the
 * destination graph and node.
 */
class Edge {
public:
  Edge(Graph *g1, Node *n1, Graph *g2, Node *n2)
      : src_graph(g1), src_node(n1), dst_graph(g2), dst_node(n2) {}

  //! A list contains pointers to Edge objects.
  typedef std::vector<Edge *> edgelist_t;

  bool validate();

  int eval(int v1, int v2);

  Graph *src_graph; /*!< Pointers to the source Graph */
  Graph *dst_graph; /*!< Pointers to the desination Graph */
  Node *src_node;   /*!< Pointers to the source Node */
  Node *dst_node;   /*!< Pointers to the desination Node */

  friend std::ostream &operator<<(std::ostream &os, const Edge &e);
};

//! Program contains references to Graphs and Nodes.
/*! Each Program has a hash table of Graphs and a list of Nodes. It's the main
 * object we need to handle with after the parser.
 */
class Program {
public:
  Program(Graph::graphtable_t &gs, Edge::edgelist_t &es)
      : graphs(gs), edges(es) {}

  Graph::graphtable_t graphs;
  Edge::edgelist_t edges;

  typedef std::map<string_t, int> report_t;

  //! Get total number of binary nodes in all graphs.
  report_t getReport();
  friend std::ostream &operator<<(std::ostream &os, const Program &dfg);
};
}

#endif
