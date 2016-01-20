
SRC=src
BIN=bin
LIB=lib
INCLUDE=include
BENCH=benchmarks

CC=gcc
CFLAGS=-I$(INCLUDE)

CXX=g++
CXXFLAGS=-I$(INCLUDE) -std=c++11 -O3

DEPS=\
	$(INCLUDE)/dfp.hh \
	$(INCLUDE)/dfp_parser.hh \
	$(INCLUDE)/dfp_lexer.hh	\
	$(INCLUDE)/dfp_program.hh \
	$(INCLUDE)/dfp_optimizer.hh \

OBJS=\
	$(BIN)/main.o \
	$(BIN)/dfp.o 	\
	$(BIN)/dfp_parser.o \
	$(BIN)/dfp_lexer.o \
	$(BIN)/dfp_program.o \
	$(BIN)/dfp_optimizer.o

$(BIN)/%.o: $(SRC)/%.cc $(DEPS)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

dfp: $(OBJS)
	$(CXX) -o $@ $(OBJS)

all: dfp

.PHONY:
	clean bench

clean:
	rm -f $(BIN)/*.o
	rm -f dfp

bench: dfp
	./dfp -f $(BENCH)/bench01_livevar.dfp
	./dfp -f $(BENCH)/bench02_defs.dfp
	./dfp -f $(BENCH)/bench03_const.dfp
	./dfp -f $(BENCH)/bench04_general.dfp
