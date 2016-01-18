
%{
#include <stdio.h>
#include "parser.hh"
%}

%option outfile="src/lex.yy.c"

%%

"dfg"           { return DFG;     }
"In"            { return IN;      }
"Out"           { return OUT;     }
"Add"           { return ADD;     }
"Sub"           { return SUB;     }
"Div"           { return DIV;     }
"Mult"          { return MULT;    }
"{"             { return LCURLY;  }
"}"             { return RCURLY;  }
"["             { return LSQUARE; }
"]"             { return RSQUARE; }
";"             { return COLON;   }
"->"            { return ARROW;   }
[a-z][a-z0-9]*  {
  yylval.str = yytext;
  return ID;
}
[0-9]+          {
  yylval.num = atoi(yytext);
  return INT;
}

[ \t\n]         { }
.               {
  fprintf(stderr, "Lexical error: %s\n", yytext);
  exit(1);
}
%%
