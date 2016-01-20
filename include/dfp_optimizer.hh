
#ifndef DFP_OPTIMIER_H__
#define DFP_OPTIMIER_H__

#include "dfp_program.hh"

namespace DFP {

//! Base optimizer class for different optimize methods.
class Optimizer {
public:
  //! Virtual function will optimize original Program to an optimized Program.
  virtual Program *optimize(Program *dfp) { return dfp; };
};

//! Do live variables dataflow analysis
class LiveVarOptimizer : public Optimizer {
public:
  Program *optimize(Program *dfp);
};

//! Do constant reduction dataflow analysis
class ConstantOptimizer : public Optimizer {
public:
  Program *optimize(Program *dfp);
};

//! Do same expression reduction dataflow analysis
class SameExprOptimizer : public Optimizer {
public:
  Program *optimize(Program *dfp);
};
}

#endif
