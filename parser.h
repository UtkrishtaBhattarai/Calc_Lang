// File: parser.h
// Purpose: Class definition of a recursive descent parser.
#include "lexer.h"
#include "parse_tree.h"

class Parser
{
public:
  // constructor
  Parser(Lexer *_lex);

  // attempt to parse the program which the lexer provides
  Parse_Tree *parse();

private:
  Lexer *_lex;

  //////////////////////////////////////////
  // Lexer Convenience Functions
  //////////////////////////////////////////

  // Returns true if the current token matches tok
  // Returns false otherwise
  bool has(Token tok);

  // Returns true if the current token matches tok
  // Prints an error message and aborts the program otherwise
  bool must_be(Token tok);

  // Return the current token and advance the lexer.
  Lexer_Token consume();

  //////////////////////////////////////////
  // Recursive Descent Parser functions
  //////////////////////////////////////////
  Parse_Tree* parse_Program();
  Parse_Tree* parse_Statement();
  Parse_Tree* parse_Statement_Body();
  Parse_Tree* parse_Statement2(Parse_Tree *left);
  Parse_Tree* parse_Statement3(Parse_Tree *left);
  Parse_Tree* parse_IO_Operation();
  Parse_Tree* parse_Expression();
  Parse_Tree* parse_Expression2(Parse_Tree *left);
  Parse_Tree* parse_Term();
  Parse_Tree* parse_Term2(Parse_Tree *left);
  Parse_Tree* parse_Factor();
  Parse_Tree* parse_Factor2(Parse_Tree *left);
  Parse_Tree* parse_Base();
  Parse_Tree* parse_Number();
  Parse_Tree* parse_Record_Decl();
  Parse_Tree* parse_Field_List(Record_Declaration *decl);
  Parse_Tree* parse_Field(Record_Declaration *decl);
  Parse_Tree* parse_Record_Inst();
  Parse_Tree* parse_Branch();
  Parse_Tree* parse_Loop();
  Parse_Tree* parse_Fun_Def();
  Parse_Tree* parse_Param_List();
  Parse_Tree* parse_Condition();
  Parse_Tree* parse_Condition2(Parse_Tree *left);
  Parse_Tree* parse_Ref();
  Parse_Tree* parse_Ref2(Parse_Tree *left);
  Parse_Tree* parse_Arg_List();
};