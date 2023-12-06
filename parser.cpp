// File: parser.cpp
// Purpose: The implemenation file for the parser class
#include "parser.h"
#include <iostream>

// constructor
Parser::Parser(Lexer *_lex)
{
  // get the lexer
  this->_lex = _lex;

  // get the first token
  _lex->next();
}

// attempt to parse the program which the lexer provides
Parse_Tree *Parser::parse() { return parse_Program(); }

//////////////////////////////////////////
// Lexer Convenience Functions
//////////////////////////////////////////

// Returns true if the current token matches tok{}
// Returns false otherwise
bool Parser::has(Token tok) { return _lex->cur().tok == tok; }

// Returns true if the current token matches tok
// Prints an error message and aborts the program otherwise
bool Parser::must_be(Token tok)
{
  if (has(tok))
  {
    return true;
  }

  // if we make it here, this is an error
  std::cerr << "Parse Error: Unexpected " << _lex->cur() << std::endl;

  // destroy the program
  exit(-1);
}

// Return the current token and advance the lexer.
Lexer_Token Parser::consume()
{
  Lexer_Token t = _lex->cur();
  _lex->next();
  return t;
}

//////////////////////////////////////////
// Recursive Descent Parser functions
//////////////////////////////////////////

/*
< Program > ::= < Statement > < Program' >

< Program' > ::=  < Statement > < Program' >
                  | ""
*/
Parse_Tree *Parser::parse_Program()
{
  Program *result = new Program();

  do
  {
    Parse_Tree *statement = parse_Statement();
    // ignore null statements
    if (statement != nullptr)
    {
      result->add(statement);
    }
  } while (not has(EOI) and not has(END));

  return result;
}

/*
< Statement >    ::= < Statement-Body > NEWLINE
*/
Parse_Tree *Parser::parse_Statement()
{
  Parse_Tree *result;

  result = parse_Statement_Body();
  must_be(NEWLINE);
  consume();

  return result;
}

/*
< Statement >    ::= ID < Statement' > NEWLINE
                     | < IO-Operation > NEWLINE
                     | < Expression > NEWLINE
                     | NEWLINE
 */
/*
Parse_Tree* Parser::parse_Statement() {
  Parse_Tree *result;

  if(has(NEWLINE)) {
    consume();
    return nullptr;
  }

  if(has(ID)) {
    // get the ID from parse_Number
    result = parse_Number();
    result = parse_Statement2(result);
  } else if(has(INPUT) or has(DISPLAY)) {
    result = parse_IO_Operation();
  } else {
    result = parse_Expression();
  }

  must_be(NEWLINE);
  consume();

  return result;
}
*/

/*
< Statement-Body > ::= < Ref > < Statement' >
                       | < IO-Operation >
                       | < Record-Decl >
                       | < Branch >
                       | < Loop >
                       | < Fun-Def >
                       | < Expression >
                       | ""
 */

Parse_Tree *Parser::parse_Statement_Body()
{
  Parse_Tree *result;

  if (has(ID))
  {

    // get the ID from parse_Number
    result = parse_Ref();
    result = parse_Statement2(result);
  }
  else if (has(INPUT) or has(DISPLAY))
  {
    result = parse_IO_Operation();
  }
  else if (has(RECORD))
  {
    result = parse_Record_Decl();
  }
  else if (has(IF))
  {
    result = parse_Branch();
  }
  else if (has(WHILE))
  {
    result = parse_Loop();
  }
  else if (has(FUN))
  {
    result = parse_Fun_Def();
  }
  else if (has(ARRAY))
  {
    result = parse_Array_Decl();
  }
  else if (not has(NEWLINE))
  {
    result = parse_Expression();
  }
  else
  {
    result = nullptr; // ""
  }

  return result;
}

/*
< Statement' >   ::= EQUAL < Expression >
                     | < Factor' > < Term' > < Expression' >
*/
/*
Parse_Tree *Parser::parse_Statement2(Parse_Tree *left)
{
  Parse_Tree *result;

  if(has(EQUAL)) {
    // this an assignment
    consume();
    Assignment *result = new Assignment();
    result->left(left);
    result->right(parse_Expression());
    return result;
  } else {
    result = parse_Factor2(left);
    result = parse_Term2(result);
    result = parse_Expression2(result);
  }

  return result;
}
*/

/*
< Statement' >   ::= EQUAL < Statement'' >
                    | < Factor' > < Term' > < Expression' >
*/
Parse_Tree *Parser::parse_Statement2(Parse_Tree *left)
{
  Parse_Tree *result;

  if (has(EQUAL))
  {
    // this an assignment
    consume();
    Assignment *result = new Assignment();
    result->left(left);
    result->right(parse_Statement3(result));
    return result;
  }

  else
  {
    result = parse_Factor2(left);
    result = parse_Term2(result);
    result = parse_Expression2(result);
  }

  return result;
}

/*
< Statement'' >  ::= < Expression >
                    | < Record-Inst >
*/
Parse_Tree *Parser::parse_Statement3(Parse_Tree *left)
{
  Parse_Tree *result;
  if (has(NEW))
  {
    result = parse_Record_Inst();
  }
  else
  {
    result = parse_Expression();
  }
  return result;
}

/*
< IO-Operation > ::= DISPLAY < Expression >
                     | INPUT ID
*/
Parse_Tree *Parser::parse_IO_Operation()
{
  if (has(DISPLAY))
  {
    consume();
    Display *result = new Display();
    result->child(parse_Expression());
    return result;
  }

  must_be(INPUT);
  consume();
  must_be(ID);
  Input *result = new Input();
  result->child(parse_Number());
  return result;
}

/*< Branch >       ::= IF < Condition > NEWLINE < Program > END IF
 */
Parse_Tree *Parser::parse_Branch()
{
  must_be(IF);
  consume();
  Branch *result = new Branch();
  result->left(parse_Condition());
  must_be(NEWLINE);
  consume();
  result->right(parse_Program());
  must_be(END);
  consume();
  must_be(IF);
  consume();
  return result;
}

/*< Loop >         ::= WHILE < Condition > NEWLINE < Program > END WHILE
 */
Parse_Tree *Parser::parse_Loop()
{
  must_be(WHILE);
  consume();
  Loop *result = new Loop();
  result->left(parse_Condition());
  must_be(NEWLINE);
  consume();
  result->right(parse_Program());
  must_be(END);
  consume();
  must_be(WHILE);
  consume();
  return result;
}

/*
< Function-Def > ::= FUN ID LPAREN < Param-List > RPAREN NEWLINE < Program > END FUN
*/
Parse_Tree *Parser::parse_Fun_Def()
{
  must_be(FUN);
  consume();
  must_be(ID);
  Lexer_Token id = consume();
  must_be(LPAREN);
  consume();
  Parse_Tree *plist = parse_Param_List();
  must_be(RPAREN);
  consume();
  must_be(NEWLINE);
  consume();
  Parse_Tree *program = parse_Program();
  must_be(END);
  consume();
  must_be(FUN);
  consume();

  Fun_Def *result = new Fun_Def(id);
  result->left(plist);
  result->right(program);
  return (Parse_Tree *)result;
}

/*
< Param-List >   ::= < NV-Param-List >
                     | ""

< NV-Param-List > ::= < NV-Param-List > COMMA ID
                      | ID
*/
Parse_Tree *Parser::parse_Param_List()
{
  Parse_List *result = new Parse_List();
  bool done;

  // check for an empty list
  if (not has(ID))
  {
    return result;
  }

  do
  {
    must_be(ID);
    result->add(new Variable(consume()));
    if (has(COMMA))
    {
      done = false;
      consume();
    }
    else
    {
      done = true;
    }
  } while (not done);

  return result;
}

/*
< Expression >   ::= < Term > < Expression' >
 */
Parse_Tree *Parser::parse_Expression()
{
  Parse_Tree *left = parse_Term();
  return parse_Expression2(left);
}

/*
< Expression' >  ::= PLUS < Term > < Expression' >
                     | MINUS < Term > < Expression' >
                     | ""
 */
Parse_Tree *Parser::parse_Expression2(Parse_Tree *left)
{
  if (has(PLUS))
  {
    consume();
    Add *result = new Add();
    result->left(left);
    result->right(parse_Term());
    return parse_Expression2(result);
  }
  else if (has(MINUS))
  {
    consume();
    Subtract *result = new Subtract();
    result->left(left);
    result->right(parse_Term());
    return parse_Expression2(result);
  }

  // "" rule
  return left;
}

/*
< Term >         ::= < Factor > < Term' >
*/
Parse_Tree *Parser::parse_Term()
{
  Parse_Tree *left = parse_Factor();
  return parse_Term2(left);
}

/*
< Term' >        ::= TIMES < Factor > < Term' >
                     | DIVIDE < Factor > < Term' >
                     | MOD < Factor > < Term' >
*/
Parse_Tree *Parser::parse_Term2(Parse_Tree *left)
{
  if (has(TIMES))
  {
    consume();
    Multiply *result = new Multiply();
    result->left(left);
    result->right(parse_Factor());
    return parse_Term2(result);
  }
  else if (has(DIVIDE))
  {
    consume();
    Divide *result = new Divide();
    result->left(left);
    result->right(parse_Factor());
    return parse_Term2(result);
  }
  else if (has(MOD))
  {
    consume();
    Mod *result = new Mod();
    result->left(left);
    result->right(parse_Factor());
    return parse_Term2(result);
  }

  // epsilon rule
  return left;
}

/*
< Factor >       ::= < Base > < Factor' >
*/
Parse_Tree *Parser::parse_Factor()
{
  Parse_Tree *left = parse_Base();
  return parse_Factor2(left);
}

/*
< Factor' >      ::= POW < Factor >
                     | ""
*/
Parse_Tree *Parser::parse_Factor2(Parse_Tree *left)
{
  if (has(POW))
  {
    consume();
    Power *result = new Power();
    result->left(left);
    result->right(parse_Factor());
    return result;
  }

  return left;
}

/*
< Base >         ::= LPAREN < Expression > RPAREN
                     | MINUS < Expression >
                     | < Number >
*/
Parse_Tree *Parser::parse_Base()
{
  if (has(LPAREN))
  {
    consume();
    Parse_Tree *result = parse_Expression();
    must_be(RPAREN);
    consume();
    return result;
  }
  else if (has(MINUS))
  {
    consume();
    Negation *result = new Negation();
    result->child(parse_Expression());
    return result;
  }
  else
  {
    return parse_Number();
  }
}

/*
< Number >       ::= INTLIT | REALLIT | < Ref >
*/
Parse_Tree *Parser::parse_Number()
{
  if (has(INTLIT))
  {
    return new Literal(consume());
  }
  else if (has(REALLIT))
  {
    return new Literal(consume());
  }
  else if (has(STRLIT))
  {
    return new Literal(consume());
  }
  else
  {
    must_be(ID);
    return parse_Ref();
  }
}

/*
< Record-Decl >  ::= RECORD ID NEWLINE < Field-List > END RECORD
 */
Parse_Tree *Parser::parse_Record_Decl()
{
  if (has(RECORD))
  {
    consume();
    must_be(ID);
    Record_Declaration *result = new Record_Declaration(consume());
    must_be(NEWLINE);
    consume();
    result->add(parse_Field_List(result));
    must_be(END);
    consume();
    must_be(RECORD);
    consume();
    return result;
  }
  return nullptr;
}

/*
< Field-List >   ::= < Field-List > < Field >
                     | < Field >
 */
Parse_Tree *Parser::parse_Field_List(Record_Declaration *decl)
{
  do
  {
    if (decl != nullptr)
    {
      decl->add(parse_Field(decl));
    }
  } while (not has(END));
  return decl;
}

/*
< Field >        ::= FIELD ID NEWLINE
 */
Parse_Tree *Parser::parse_Field(Record_Declaration *decl)
{
  Parse_Tree *result;
  must_be(FIELD);
  consume();
  must_be(ID);
  result = new Variable(consume());
  must_be(NEWLINE);
  consume();
  return result;
}

/*
< Record-Inst >  ::= NEW ID
 */
Parse_Tree *Parser::parse_Record_Inst()
{
  Parse_Tree *result;
  must_be(NEW);
  consume();
  must_be(ID);
  result = new Variable(consume());
  return result;
}

/*
< Condition >    ::= < Expression > < Condition' >
 */
Parse_Tree *Parser::parse_Condition()
{
  Parse_Tree *result;
  result = parse_Expression();
  result = parse_Condition2(result);
  return result;
}

/*
< Condition' >   ::= EQUAL < Expression >
                     | NE < Expression >
                     | LT  < Expression >
                     | GT  < Expression >
                     | LTE  < Expression >
                     | GTE  < Expression >
 */
Parse_Tree *Parser::parse_Condition2(Parse_Tree *left)
{
  BinaryOp *result;
  if (has(EQUAL))
  {
    consume();
    result = new Equal();
  }
  else if (has(NE))
  {
    consume();
    result = new Not_Equal();
  }
  else if (has(LT))
  {
    consume();
    result = new Less();
  }
  else if (has(GT))
  {
    consume();
    result = new Greater();
  }
  else if (has(LTE))
  {
    consume();
    result = new Less_or_Equal();
  }
  else if (must_be(GTE))
  {
    consume();
    result = new Greater_or_Equal();
  }

  result->left(left);
  result->right(parse_Expression());
  return result;
}

/*
< Ref >          ::= ID < Ref' >
 */
Parse_Tree *Parser::parse_Ref()
{
  must_be(ID);
  Lexer_Token lx = _lex->cur();
  Parse_Tree *left = new Variable(consume());
  if (has(DOT))
  {
    consume();
    if (has(SET))
    {
      consume();
      if (has(ID) or has(INTLIT))
      {
        ArrayAssignment *result = new ArrayAssignment();
        result->left(left);
        result->right(parse_Statement3(result));
        return result;
      }
    }

    else if (has(GET))
    {
      consume();
      if (has(ID) or has(INTLIT))
      {
        std::string str =  _lex->cur().lexeme; 
        int arr_index = std::stoi(str);
        consume();
        Array_Access *result = new Array_Access(lx,arr_index);
        return result;
      }

    }
    else if (has(SIZE))
    {
      consume();
      Array_Size *result = new Array_Size(lx);
      return result;
    }

    else if (has(UPDATE))
    {

      consume();
      Lexer_Token arr_index = _lex->cur();
      consume();
      Lexer_Token update_value = _lex->cur();
      consume();
      Array_Update *result = new Array_Update(lx, arr_index, update_value);
      return result;
    }
  }
  return parse_Ref2(left);
}

/*
< Ref' >         ::= DOT ID < Ref' >
                     | LPAREN < Arg-List > RPAREN
                     | ""
 */
Parse_Tree *Parser::parse_Ref2(Parse_Tree *left)
{
  if (has(DOT))
  {
    consume();
    must_be(ID);
    Record_Access *result = new Record_Access();
    result->left(left);
    result->right(new Variable(consume()));

    return parse_Ref2(result);
  }
  else if (has(LPAREN))
  {
    consume();
    Parse_Tree *alist = parse_Arg_List();
    Fun_Call *result = new Fun_Call();
    result->left(left);
    result->right(alist);
    must_be(RPAREN);
    consume();
    return result;
  }

  // null case reaches here
  return left;
}

/*
< Arg-List >   ::= < NV-Arg-List >
                     | ""

< NV-Arg-List > ::= < NV-Arg-List > COMMA < Expression >
                      | < Expression >
*/
Parse_Tree *Parser::parse_Arg_List()
{
  Parse_List *result = new Parse_List();
  bool done;

  // check for an empty list
  if (has(RPAREN))
  {
    return result;
  }

  do
  {
    result->add(parse_Expression());
    if (has(COMMA))
    {
      done = false;
      consume();
    }
    else
    {
      done = true;
    }
  } while (not done);

  return result;
}

Parse_Tree *Parser::parse_Array_Decl()
{
  must_be(ARRAY);
  consume();
  must_be(OF);
  consume();
  Lexer_Token typeToken = consume();
  must_be(WITH);
  consume();
  must_be(BOUNDS);
  consume();
  must_be(LBRACKET);
  consume();

  if (has(ID))
  {
    std::cout << "Hello world " << std::endl;
  }
  int bounds = std::stoi(_lex->cur().lexeme);
  consume();
  must_be(RBRACKET);
  consume();
  Lexer_Token arrayName = consume();

  return new Array_Declaration(typeToken, bounds, arrayName);
}
