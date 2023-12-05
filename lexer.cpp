// File: lexer.cpp
// Purpose: Implementation for a lexer.
#include "lexer.h"
#include <ctype.h>
#include <iostream>
#include <map>
#include <string>

////////////////////////////////////////
// Lexer_Token implementation
////////////////////////////////////////
Lexer_Token::Lexer_Token() {
  // do nothing
}

Lexer_Token::Lexer_Token(Token tok, const std::string &lexeme, int line,
                         int col) {
  this->tok = tok;
  this->lexeme = lexeme;
  this->line = line;
  this->col = col;
}

// print out the lexer token
std::ostream &operator<<(std::ostream &os, const Lexer_Token &t) {
  static std::string token_label[] = {
      "INVALID", "EOI", "NEWLINE", "PLUS",   "MINUS",  "TIMES",  "DIVIDE",
      "MOD",     "POW", "LPAREN",  "RPAREN", "INTLIT", "REALLIT", "EQUAL",
       "DISPLAY", "INPUT", "ID", "DOT", "NEW", "RECORD", "END", "FIELD",
        "IF", "WHILE", "NE", "LT", "GT", "LTE", "GTE", "FUN", "COMMA", "CLASS", "STRLIT","ARRAY", "LBRACKET",
  "RBRACKET",};
  return os << token_label[t.tok] << " \"" << t.lexeme << "\" Line: " << t.line
            << " Column " << t.col;
}

////////////////////////////////////////
// Lexer Implementation
////////////////////////////////////////
Lexer::Lexer(std::istream &_is) : _is(_is) {
  _cur.tok = INVALID;
  _line = 1;
  _col = 0;

  // read the first character
  read();
}

// return the next token in the stream
Lexer_Token Lexer::next() {
  // skip to the next token
  skip();

  // handle end of file
  if(not _is) {
    _cur = Lexer_Token(EOI, "", _line, _col);
    return _cur;
  }
  
  // initialize the next token
  _cur = Lexer_Token(INVALID, "", _line, _col);

  if (lex_single()) {
    // nothing to do
  }
  else if(lex_string())
  {
    //norbg
  } else if (lex_number()) {
    // nothing to do
  } else if(lex_fixed()) {
    // nothing to do
  } else if(lex_kw_or_id()) {
    // nothing to do
  } else {
    // consume the invalid character
    consume();
  }

  return _cur;
}

// return the current token
Lexer_Token Lexer::cur() { return _cur; }

// get the next character from the stream
void Lexer::read() {
  // handle the start of new lines
  if (_cur_char == '\n') {
    _line++;
    _col = 0;
  }

  // read a character
  _cur_char = _is.get();
  if (_is) {
    // increment the column if we have read the character
    _col++;
  }
}

// consume a character after it is matched
void Lexer::consume() {
  _cur.lexeme += _cur_char;
  read();
}

// skip insignificant / non-token input
void Lexer::skip() {
  while(_is && _cur_char != '\n' && isspace(_cur_char)) {
    read();
  }

  // skip comments
  if(_cur_char == '#') {
    while(_is && _cur_char != '\n') {
      read();
    }
  }
}

// attempt to match a single character token, return true on success
bool Lexer::lex_single() {
  // build our map of tokens
  std::map<char, Token> tokens;
  tokens['\n'] = NEWLINE;
  tokens['+'] = PLUS;
  tokens['-'] = MINUS;
  tokens['*'] = TIMES;
  tokens['/'] = DIVIDE;
  tokens['^'] = POW;
  tokens['('] = LPAREN;
  tokens[')'] = RPAREN;
  tokens['='] = EQUAL;
  tokens['.'] = DOT;
  tokens[','] = COMMA;
  tokens['['] = LBRACKET;
  tokens[']'] = RBRACKET;

  // search for the current character in our map
  auto itr = tokens.find(_cur_char);
  if (itr == tokens.end()) {
    // no match
    return false;
  }

  // we have matched a token
  consume();
  _cur.tok = itr->second;

  return true;
}

// attempt to match a number
bool Lexer::lex_number() {
  // verify that we start with a digit
  if (!isdigit(_cur_char))
    return false;

  // we at least have an integer
  _cur.tok = INTLIT;

  // consume the digits
  while (isdigit(_cur_char)) {
    consume();
  }

  if (_cur_char != '.') {
    // we have matched an integer
    return true;
  }

  // consume the .
  consume();

  // transition to an invalid token
  _cur.tok = INVALID;

  if (not isdigit(_cur_char)) {
    // we have matched invalid
    return true;
  }

  // transition to a real literal
  _cur.tok = REALLIT;

  // consume the digits
  while (isdigit(_cur_char)) {
    consume();
  }

  return true;
}

// attempt to match a keyword or an id
bool Lexer::lex_kw_or_id() {
  std::map<std::string, Token> tokens;
  tokens["MOD"] = MOD;
  tokens["display"] = DISPLAY;
  tokens["input"] = INPUT;
  tokens["new"] = NEW;
  tokens["record"] = RECORD;
  tokens["end"] = END;
  tokens["field"] = FIELD;
  tokens["if"] = IF;
  tokens["while"] = WHILE;
  tokens["fun"] = FUN;
  tokens["class"] = CLASS;
  tokens["array"] = ARRAY;

  // check to see if it starts properly
  if(_cur_char != '_' and not isalpha(_cur_char)){return false;}

  // consume all of the alpha numeric characters and _s
  while(isalnum(_cur_char) or _cur_char == '_') { consume(); }

  auto itr = tokens.find(_cur.lexeme);
  if(itr != tokens.end()) {
    _cur.tok = itr->second;
  } else {
    _cur.tok = ID; 
  }

  return true;
}

// Attempt to match a string
bool Lexer::lex_string()
{
  // verify that we start with a double quote
  if (_cur_char != '"')
    return false;
  consume();
  _cur.tok = STRLIT;

  // consume characters until the closing double quote
  while (_cur_char != '\0' && _cur_char != '"')
  {
    consume();
  }

  if (_cur_char != '"')
  {
    _cur.tok = INVALID;
  }
  else
  {
    consume();
  }
  _cur.lexeme = _cur.lexeme.substr(1, _cur.lexeme.size() - 2);

  return true;
}

  
// attempt to match a fixed-width unconstrained token
bool Lexer::lex_fixed()
{
  // check for <
  if(_cur_char == '<') {
    _cur.tok = LT;
    consume();
    if(_cur_char == '>') {
      _cur.tok = NE;
      consume();
    } else if(_cur_char == '=') {
      _cur.tok = LTE;
      consume();
    }
    return true;
  } else if(_cur_char == '>') {
    _cur.tok = GT;
    consume();
    if(_cur_char == '=') {
      _cur.tok = GTE;
      consume();
    }

    return true;
  }

  return false;
}