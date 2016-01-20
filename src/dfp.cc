
#include <iostream>
#include <fstream>
#include <string>

#include <cstdio>

#include <queue>
#include <map>

#include "dfp.hh"
#include "dfp_lexer.hh"
#include "dfp_program.hh"
#include "dfp_parser.hh"

namespace DFP {
int Compiler::compile() {
  FILE *fp = fopen(file_name.c_str(), "r");
  if (!fp) {
    fprintf(stderr, "Error: cannot open file %s.\n", file_name.c_str());
    return 1;
  }

  Parser parser(new Lexer(fp));
  dfp = parser.program();

  std::cout << "parsed " << dfp->graphs.size() << " graphs and "
            << dfp->edges.size() << " edges" << std::endl;

  return 0;
}

int Compiler::optimize() {
  // Optimize pass #1: remove useless nodes
  for (auto gp : dfp->graphs) {
    Graph *g = gp.second;
    std::map<string_t, bool> bitmap;
    std::queue<string_t> q;

    // initialize;
    for (auto np : g->nt) {
      bitmap[np.first] = false;
    }

    nodetable_t o_nodes = g->getOutNodeTable();

    for (auto np : o_nodes) {
      bitmap[np.first] = true;
      q.push(np.first);
    }

    while (!q.empty()) {
      Node *n = g->nt[q.front()];
      q.pop();
      bitmap[n->id] = true;
      for (Value *v : n->vl) {
        if (v->type == StrType) {
          q.push(((StrValue *)v)->value);
        }
      }
    }

    for (auto bp : bitmap) {
      if (!bp.second) {
        // remove the node ..
        g->nt.erase(bp.first);
      }
    }
  }

  // Optimize pass #2: remove same expressions
  for (auto gp : dfp->graphs) {
    Graph *g = gp.second;
    bool is_changed = false;
    do {
      is_changed = false;
      for (auto np1 : g->nt) {
        Node *n1 = np1.second;
        if (!n1->isBinary())
          continue;

        for (auto np2 : g->nt) {
          Node *n2 = np2.second;
          if (n2->id != n1->id) {
            if (*n1 == *n2) {
              // apply change
              for (auto np3 : g->nt) {
                Node *n3 = np3.second;
                if (n3->id == n1->id || n3->id == n2->id)
                  continue;

                for (int i = 0; i < n3->vl.size(); i++) {
                  Value *v = n3->vl[i];
                  if (v->type == StrType) {
                    StrValue *sv = (StrValue *)v;
                    if (sv->value == n2->id) {
                      // by creating a new node, replaced the old one
                      n3->vl[i] = new StrValue(new Word(n1->id.c_str(), ID));
                    }
                  }
                }
              }
              // remove node
              g->nt.erase(n2->id);

              is_changed = true;
            }
          }
        }
      }
    } while (is_changed);
  }

  return 0;
}

int Compiler::print(std::string out_file_name) {
  std::ofstream fout(out_file_name.c_str());
  if (!fout.is_open()) {
    fprintf(stderr, "Error: cannot open output file: %s\n",
            out_file_name.c_str());
    return 1;
  }

  fout << *dfp << std::endl;
  return 0;
}
}
