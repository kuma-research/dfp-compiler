
SRC=src
BIN=bin
INCLUDE=include

YACC=yacc
LEX=lex

CC=gcc
CFLAGS=-I$(SRC)

CXX=g++
CXXFLAGS=-L/usr/local/lib -I$(INCLUDE) -I$(SRC) -ll

all: parser

$(SRC)/lex.yy.c: $(SRC)/lexer.l
	$(LEX) $(SRC)/lexer.l

$(SRC)/parser.cc: $(SRC)/parser.y
	$(YACC) --debug -d $(SRC)/parser.y -o $(SRC)/parser.cc

$(BIN)/lex.yy.o: $(SRC)/lex.yy.c $(SRC)/parser.hh
	$(CC) -c $(SRC)/lex.yy.c -o $(BIN)/lex.yy.o $(CFLAGS)

parser: $(SRC)/parser.cc $(BIN)/lex.yy.o
	$(CXX) -o $@ $(SRC)/parser.cc $(BIN)/lex.yy.o $(CXXFLAGS)

clean:
	rm $(BIN)/*
	rm $(SRC)/*.cc $(SRC)/*.hh $(SRC)/*.c
	rm parser
