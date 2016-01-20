# Tiny DFP Compiler

This is a tiny DFP compiler, which can take DFP codes input and output optimized ones.

## Introduction

This compiler follows the traditional compiler architecture:

- **Lexer**: Transforms input DFP codes into token stream.
- **Parser**: Use token stream to build DFGs (dataflow graph) and edges. Invalid semantics will cause fatal error while parsing.
- **Optimizer**: Including three different optimizers, which are based on canonical simplified data flow analysis algorithms.
- **Printer**: Print the DFP codes from DFGs and edges.

Documentations can be found in `doxygen/`.

### Codes Structure

This compiler follows the traditional C++ directory structure:

1. `src/`: C++ source code files, with suffixes `.cc`
2. `include/`: C++ header files, with suffixes `.hh`
3. `bin/`: C++ objects.
4. `Makefile`: Makefile for this compiler.

There're 4 different modules in this compiler:

1. `dfp_lexer.hh(cc)`: `class Lexer`'s definition and implementations.
2. `dfp_parser.hh(cc)`: `class Parser`'s definition and implementations.
3. `dfp_program.hh(cc)`: Internal data structure for DFP, including classes like `Graph`, `Node`, `Value` and `Edge`.
4. `dfp.hh(cc)`: Main class `Compiler`.

### System Requirements

1. C++11 enabled compiler, g++ version >= 4.8 is recommended.
2. No other libraries! Yah!

## How to Install

Just use `make all` to build the compiler and other utilities.

## Usage

### Compiler

The compiler program is `dfp`. You could simply run it from the command line.

Options:
1. `-f [filename]`: take input DFP codes from file `[filename]`
2. `-o [filename]`: output transformed DFP codes to file `[filename]`

## Test and Benchmarks

There're several benchmarks and examples in the `benchmarks/` directory. Fill free to try them.

And a brief optimization conclusion will be appended:

```
$ ./dfp -f benchmarks/bench04_general.dfp
parsed 2 graphs and 0 edges
Opt Report:	Format=[graph #id](Binary nodes number) ...
Opt pass #0:	g1(5)	g2(3)
Opt pass #1:	g1(5)	g2(3)
Opt pass #2:	g1(4)	g2(2)
Opt pass #3:	g1(1)	g2(2)
```

In this example, there're two graphs (g1 and g2), and the initial number of binary nodes are 5 and 3. After 3 passes of optimization(#0 is the original one), there're 1 and 2 number of binary nodes left in g1 and g2.
