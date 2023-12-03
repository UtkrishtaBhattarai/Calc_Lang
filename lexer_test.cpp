#include <iostream>
#include "lexer.h"

int main() {
  Lexer lex(std::cin);
  
  for(;;) {
    std::cout << lex.next() << std::endl;
  }
}
