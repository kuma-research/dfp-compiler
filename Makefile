
SRC=src
BIN=bin
LIB=lib
INCLUDE=include


CC=gcc
CFLAGS=-I$(INCLUDE)

CXX=g++
CXXFLAGS=-I$(INCLUDE) -std=c++11

DEPS=\
	$(INCLUDE)/dfp.hh \
	$(INCLUDE)/dfp_parser.hh \
	$(INCLUDE)/dfp_lexer.hh	\
	$(INCLUDE)/dfp_program.hh

OBJS=\
	$(BIN)/main.o \
	$(BIN)/dfp.o 	\
	$(BIN)/dfp_parser.o \
	$(BIN)/dfp_lexer.o \
	$(BIN)/dfp_program.o

$(BIN)/%.o: $(SRC)/%.cc $(DEPS)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

dfp: $(OBJS)
	$(CXX) -o $@ $(OBJS)

all: dfp
