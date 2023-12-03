// File: parser_test.cpp
// Purpose: A small program to test the parser
#include <iostream>
#include "parser.h"
#include "lexer.h"

int main()
{
  Lexer *lex = new Lexer(std::cin);
  Parser parser(lex);

  parser.parse()->print(0);
}