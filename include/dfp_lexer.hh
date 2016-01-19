
#ifndef DFP_LEXER_H__
#define DFP_LEXER_H__

#include <map>

namespace DFP {

class Word;

typedef char *string_t;
typedef std::map<string_t, Word *> symtable_t;

enum Tag {
  DFG = 256,
  IN,
  OUT,
  ADD,
  SUB,
  DIV,
  MULT,
  NUM,
  ID,
  ARROW,
  LSQUARE,
  RSQUARE,
  LCURLY,
  RCURLY,
  COLON
};

class Token {
public:
  int tag;

  Token(int t) : tag(t) {}
};

// Num handles int numbers
class Num : public Token {
public:
  int value;
  Num(int v) : value(v), Token(NUM) {}
};

// Word handles reserved words, identifiers, ...
class Word : public Token {
public:
  string_t lexeme;
  Word(const char *v, Tag t) : lexeme((char *)v), Token(t) {}

  static Word *ls, *rs, *lc, *rc;
  static Word *arrow, *colon;
};

class Lexer {
public:
  Lexer(FILE *fp) : in(fp) {
    peek = ' ';

    reserve(new Word("dfg", DFG));
    reserve(new Word("In", IN));
    reserve(new Word("Out", OUT));
    reserve(new Word("Add", ADD));
    reserve(new Word("Sub", SUB));
    reserve(new Word("Mult", MULT));
    reserve(new Word("Div", DIV));
  }

  void reserve(Word *w);

  void readch();
  bool readch(char c);
  Token *scan();

private:
  // file read related
  FILE *in;
  char peek;
  static int line;

  symtable_t words;
};
}

#endif
