
#ifndef DFP_H__
#define DFP_H__

#include "dfp_program.hh"
#include "dfp_optimizer.hh"

namespace DFP {

/** The Compiler class.
 *  Including several functions to finish all the compilation passes.
 */
class Compiler {
public:
  //! Constructor
  /*!
    This constructor only "caches" the input file name. And also initialize a
    list of optimizers that will be applied later.
    \param file_name the input file
  */
  Compiler(std::string file_name) : file_name(file_name) {
    ops.push_back(new LiveVarOptimizer());
    ops.push_back(new SameExprOptimizer());
    ops.push_back(new ConstantOptimizer());
  }

  //! Procedure to compile the input DFP codes
  /*!
    The compile output will be stored in *dfp for futher optimization.
    \return error code
  */
  int compile();
  //! Procedure to do optimization.
  /*!
    Futher process on stored Program *dfp.
    \return error code
  */
  int optimize();
  //! Procedure to print out the optimized DFP.
  /*!
    A formatted DFP program will be printed to out_file_name.
    \param out_file_name file name for output
    \return error code
  */
  int print(std::string out_file_name);

  std::string file_name;        /**< Input DFP codes file name */
  Program *dfp;                 /**< Compiled DFG data structure. */
  std::vector<Optimizer *> ops; /**< Optimizers that will applied here */
};
}

#endif
