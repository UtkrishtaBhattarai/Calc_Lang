// File: lexer.h
// Purpose: Definitions for a lexer.
#ifndef LEXER_H
#define LEXER_H
#include <string>
#include <iostream>

// our language's tokens
enum Token {
  INVALID = 0,
  EOI,
  NEWLINE,
  PLUS,
  MINUS,
  TIMES,
  DIVIDE,
  MOD,
  POW,
  LPAREN,
  RPAREN,
  INTLIT,
  REALLIT,
  EQUAL,
  DISPLAY,
  INPUT,
  ID,
  DOT,
  NEW,
  RECORD,
  END,
  FIELD,
  IF,
  WHILE,
  NE,
  LT,
  GT,
  LTE,
  GTE,
  FUN,
  COMMA,
  CLASS,
  STRLIT,
  ARRAY,
  LBRACKET,
  RBRACKET,
  OF, 
  WITH,
  BOUNDS,
  SET,
  GET,
  SIZE,
  UPDATE,
  LOAD,
  FETCH,
  EMPLOYEE,
  CUSTOMER,
  CUSTOMER_PURCHASE,
  WRITE,
  CLOSE,
  INHERITS,
  OBJECT,
  OBJ,
  PRIVATE,
  PUBLIC,
};

// Tokens as emitted by the lexer
class Lexer_Token
{
public:
  Lexer_Token();
  Lexer_Token(Token tok, const std::string &lexeme, int line, int col);
  Token tok;
  std::string lexeme;
  int line;
  int col;
};
std::ostream &operator<<(std::ostream &os, const Lexer_Token &t);

// Lexer definition
class Lexer
{
public:
  Lexer(std::istream &_is);

  //return the next token in the stream
  Lexer_Token next();

  //return the current token
  Lexer_Token cur();

private:
  std::istream &_is;
  char _cur_char;
  Lexer_Token _cur;
  int _line;
  int _col;

  // get the next character from the stream
  void read();

  // consume a character after it is matched
  void consume();

  // skip insignificant / non-token input
  void skip();

  // attempt to match a single character token, return true on success
  bool lex_single();

  // attempt to match a number
  bool lex_number();

  // attempt to match a keyword or an id
  bool lex_kw_or_id();

  // attempt to match a fixed-width unconstrained token
  bool lex_fixed();

  // attempt to match a string
  bool lex_string();

};

#endif