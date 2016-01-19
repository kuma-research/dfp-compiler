
#include <iostream>
#include <cctype>
#include <string>

#include "dfp_lexer.hh"

namespace DFP {

Word *Word::ls = new Word("[", LSQUARE);
Word *Word::rs = new Word("]", RSQUARE);
Word *Word::lc = new Word("{", LCURLY);
Word *Word::rc = new Word("}", RCURLY);
Word *Word::arrow = new Word("->", ARROW);
Word *Word::colon = new Word(";", COLON);

int Lexer::line = 1;
void Lexer::reserve(Word *w) { words[w->lexeme] = w; }

void Lexer::readch() { peek = fgetc(in); }
bool Lexer::readch(char c) {
  readch();
  if (peek != c)
    return false;
  peek = ' ';
  return true;
}

Token *Lexer::scan() {
  for (;; readch()) {
    if (peek == ' ' || peek == '\t')
      continue; // ignore all whitespaces
    else if (peek == '\n')
      line = line + 1;
    else
      break;
  }
  // check input char
  switch (peek) {
  case '-':
    if (readch('>')) // ->
      return Word::arrow;
    break;
  }
  // if the first letter is digit
  if (isdigit(peek)) {
    int v = 0;
    do {
      v = 10 * v + peek - '0';
      readch();
    } while (isdigit(peek));
    return new Num(v);
  }
  // if the first letter is alpha
  if (isalpha(peek)) {
    std::string buf;
    do {
      buf.push_back(peek);
      readch();
    } while (isalpha(peek) ||
             isdigit(peek)); // remain the same state if alphanumeric
    const char *ss = buf.c_str();

    std::cout << buf << std::endl;
    Word *w = words[(char *)ss];
    if (!w) {
      std::cout << buf << " is reserved" << std::endl;
      return w;
    }

    w = new Word(ss, ID);
    words[(char *)ss] = w;
    return w;
  }

  Token *tok = new Token(peek);
  peek = ' ';
  return tok;
}
}
