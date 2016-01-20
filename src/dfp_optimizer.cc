
#include <iostream>

#include <map>
#include <queue>
#include <vector>

#include "dfp_lexer.hh"
#include "dfp_optimizer.hh"

namespace DFP {

Program *LiveVarOptimizer::optimize(Program *dfp) {
  // For each graph in this DFP program
  for (auto gp : dfp->graphs) {
    Graph *g = gp.second;
    // Output nodes for Graph g.
    Node::nodetable_t o_nodes = g->getOutNodeTable();
    // bitmap will mark whether an id is useless (false).
    std::map<string_t, bool> bitmap;
    // q stores all the nodes that waited to be checked.
    std::queue<string_t> q;

    bool is_changed = false;
    do {
      // Initialize this iteration
      is_changed = false;
      bitmap.clear();
      for (auto np : g->nt)
        bitmap[np.first] = false;

      // For those output nodes, will be initially enqueued.
      for (auto np : o_nodes) {
        bitmap[np.first] = true;
        q.push(np.first);
      }

      // While there's remaining tasks.
      while (!q.empty()) {
        Node *n = g->nt[q.front()];
        q.pop();
        // Node in the queue will be marked as true.
        bitmap[n->id] = true;
        for (Value *v : n->vl) {
          // Try to put other nodes, constants will be ignored.
          if (v->type == Value::StrType) {
            string_t id = ((StrValue *)v)->value;
            if (!bitmap[id])
              q.push(id);
          }
        }
      }

      // remove nodes.
      for (auto bp : bitmap)
        if (!bp.second) {
          std::cout << bp.first << std::endl;
          is_changed = true;
          g->nt.erase(bp.first);
        }
    } while (is_changed);
  }

  return dfp; // Here this dfp is the original one. But several changes has been
              // made.
}

Program *ConstantOptimizer::optimize(Program *dfp) {
  for (auto gp : dfp->graphs) {
    Graph *g = gp.second;
    bool is_changed = false;
    do {
      is_changed = false;
      for (auto np1 : g->nt) {
        Node *n1 = np1.second;
        // Not binary nodes are ignored.s
        if (!n1->isBinary())
          continue;

        // Get two value in the binary Node's value list
        Value *v1 = n1->vl[0];
        Value *v2 = n1->vl[1];

        // Should both be integer.
        if (!(v1->type == Value::IntType && v2->type == Value::IntType))
          continue;

        // Evaluate the constants.
        IntValue *iv1 = (IntValue *)v1;
        IntValue *iv2 = (IntValue *)v2;
        int result = n1->eval(iv1->value, iv2->value);

        // replace those Values that has n1->id as value with new IntValue that
        // contains result.
        for (auto np2 : g->nt) {
          Node *n2 = np2.second;
          if (n2->id == n1->id)
            continue;
          for (int i = 0; i < n2->vl.size(); i++) {
            Value *v = n2->vl[i];
            if (v->type == Value::StrType && ((StrValue *)v)->value == n1->id) {
              // by creating a new node, replaced the old one
              n2->vl[i] = IntValue::create(result);
              is_changed = true;
            }
          }
        }
        g->nt.erase(n1->id);
        break;
      }
    } while (is_changed);
  }
  return dfp;
}

Program *SameExprOptimizer::optimize(Program *dfp) {
  for (auto gp : dfp->graphs) {
    Graph *g = gp.second;
    bool is_changed = false;
    do {
      is_changed = false;
      for (auto np1 : g->nt) {
        for (auto np2 : g->nt) {
          Node *n1 = np1.second;
          Node *n2 = np2.second;
          // If the first node is not binary or these two nodes are equal in id
          // or content (== has been overloaded for Node). Just continue.
          if (!n1->isBinary() || n2->id == n1->id || !(*n1 == *n2))
            continue;

          // If not, then should replace n2 with n1 in other node's Value list.
          for (auto np3 : g->nt) {
            Node *n3 = np3.second;
            // This node should not be equal to either of n1 and n2.
            if (n3->id == n1->id || n3->id == n2->id)
              continue;

            for (int i = 0; i < n3->vl.size(); i++) {
              Value *v = n3->vl[i];
              if (v->type == Value::StrType &&
                  ((StrValue *)v)->value == n2->id) {
                // by creating a new node, replaced the old one
                n3->vl[i] = StrValue::create(n1->id);
              }
            }
          }
          // remove node
          g->nt.erase(n2->id);
          is_changed = true;
        }
      }
    } while (is_changed);
  }
  return dfp;
}
}
