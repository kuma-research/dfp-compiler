
/* file: src/parser.y - Bison parser BNF definition for DFP language */

%{
#include <stdio.h>
#include <stdlib.h>

// top declarations
int yyerror(const char *s);

extern int yydebug;
extern "C"
{
  int yyparse(void);
  int yylex(void);
  int yywrap() { return 1; }
}
%}

%union {
  int num;
  const char* str;
}

%token <num> INT        /* 32-bit integer available             */
%token <str> ID         /* an identifier for a dfg              */
%token DFG              /* the "dfg" const string               */
%token NTYPE            /* an enum with In Out Add Mult Div Sub */
%token COLON            /* ;  */
%token ARROW            /* -> */
%token LCURLY           /* {  */
%token RCURLY           /* }  */
%token LSQUARE          /* [  */
%token RSQUARE          /* ]  */
%token IN OUT           /* In and Out node types */
%token ADD SUB MULT DIV /* Calculation node types */
%%

program:
    graphlist edgelist
  ;

graphlist:
    graph
  | graph graphlist
  ;
/* dfg graph */
graph:
    DFG ID LCURLY nodelist RCURLY COLON
  ;

/* dfg node type */
nodetype:
    IN | OUT | ADD | SUB | MULT | DIV
  ;

/* dfg list of nodes */
nodelist:
    node
  | node nodelist
  ;

node:
    nodetype ID LSQUARE valuelist RSQUARE COLON
  ;

valuelist:
  | value valuelist
  ;

/* dfg input or output value */
value:
    INT
  | ID
  ;

edgelist:
  | edge edgelist
  ;

edge:
    ID LSQUARE ID RSQUARE ARROW ID LSQUARE ID RSQUARE COLON
  ;

%%

int yyerror(const char *s) {
  extern char *yytext;
  fprintf(stderr, "error: %s\n", yytext);
  return 0;
}

int main(int argc, char *argv[]) {
  extern int yyparse(void);
  extern FILE *yyin;

  yydebug = 1;
  yyin = stdin;
  if (yyparse()) {
    fprintf(stderr, "Error!\n");
    exit(1);
  }

  return 0;
}
