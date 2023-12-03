// File: calc.cpp
// Purpose: This is an implementation of the calc interpreter
#include "lexer.h"
#include "parse_tree.h"
#include "parser.h"
#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <fstream>

// REPL (Read Execute Print Loop) interface
void calc_repl();

// Run the contents of a file
void calc_run(const std::string &filename);

int main(int argc, char **argv) {
  if(argc == 1) {
    calc_repl();
  } else {
    calc_run(argv[1]);
  }
}

// REPL (Read Execute Print Loop) interface
void calc_repl() {
  std::string line;
  Ref_Env env; //global environment

  while (std::cin) {
    // read the line
    std::cout << "> ";
    while(std::cin.peek() == '\n') { std::cin.get();}
    getline(std::cin, line);
    if (not std::cin)
      continue;

    // create the stream
    std::istringstream is(line + "\n");

    // make our lexer and parser
    Lexer lexer(is);
    Parser parser(&lexer);
    Parse_Tree *program = parser.parse();

    // run the program and display the result
    EvalResult result = program->eval(&env);
    if(result.type() == INTEGER) {
      std::cout << result.as_integer() << std::endl;
    } else if(result.type() == REAL) {
      std::cout << result.as_real() << std::endl;
    }

    // cleanup
    delete program;
  }
}


// Run the contents of a file
void calc_run(const std::string &filename)
{
  std::ifstream file;
  file.open(filename);
  if(!file) {
    std::cerr << "Could not open file: " << filename << std::endl;
    return;
  }

  // create a lexer/parser for the file
  Lexer lexer(file);
  Parser parser(&lexer);
  Parse_Tree *program = parser.parse();

  // run the program
  Ref_Env env;
  program->eval(&env);

  delete program;
}