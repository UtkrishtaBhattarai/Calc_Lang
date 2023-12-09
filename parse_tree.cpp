// File: parse_tree.cpp
// Purpose: Implementation of the parse tree classes
#include "parse_tree.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

// Closure Constructor
Closure::Closure(Fun_Def *fun, Ref_Env *env)
{
  this->fun = fun;
  this->env = env;
}

//////////////////////////////////////////
// Evaluation Results
//////////////////////////////////////////
EvalResult::EvalResult() { this->_type = VOID; }

void EvalResult::set_type(EvalType _type)
{
  this->_type = _type;
  this->_i = 0;
  this->_d = 0;
}

// set the value and infer the type
void EvalResult::set(int _i)
{
  this->_i = _i;
  _type = INTEGER;
}

void EvalResult::set(std::string _str)
{
  this->_str = _str;
  _type = STRING;
}

void EvalResult::set(double _d)
{
  this->_d = _d;
  _type = REAL;
}

void EvalResult::set(bool _b)
{
  this->_b = _b;
  _type = BOOLEAN;
}

void EvalResult::set(Closure *_fun)
{
  this->_fun = _fun;
  _type = FUNCTION;
}

void EvalResult::set(std::vector<int> _myarray)
{
  this->_myarray = _myarray;
  _type = VECTOR;
}

// type coercion functions
int EvalResult::as_integer()
{

  if (_type == INTEGER)
  {
    return _i;
  }
  else
  {
    return (int)(_d);
  }
}

std::string EvalResult::as_string()
{
  if (_type == STRING)
  {
    return _str;
  }
  else
  {
    return std::string(_str);
  }
}

double EvalResult::as_real()
{
  if (_type == REAL)
  {
    return _d;
  }
  else
  {
    return (double)_i;
  }
}

bool EvalResult::as_bool()
{
  return _b;
}

Closure *EvalResult::as_fun()
{
  return _fun;
}

std::vector<int> EvalResult::as_array()
{
  if (_type == VECTOR && !_myarray.empty())
  {
    return _myarray;
  }
  // Handle the case where the type is not an array or the array is empty
  // You can return an empty vector or throw an error depending on your design
  return std::vector<int>();
}

// retrieve the type
EvalType EvalResult::type() { return _type; }

//////////////////////////////////////////
// Base classes for the operators
//////////////////////////////////////////
Parse_Tree::~Parse_Tree()
{
  // nothing to do
}

// destructor
UnaryOp::~UnaryOp() { delete child(); }

// accessor and mutator for the child
Parse_Tree *UnaryOp::child() const { return _child; }

void UnaryOp::child(Parse_Tree *_child) { this->_child = _child; }

BinaryOp::~BinaryOp()
{
  delete left();
  delete right();
}

// accessor and mutator for the children
Parse_Tree *BinaryOp::left() const { return _left; }

void BinaryOp::left(Parse_Tree *_left) { this->_left = _left; }

Parse_Tree *BinaryOp::right() const { return _right; }

void BinaryOp::right(Parse_Tree *_right) { this->_right = _right; }

NaryOp::~NaryOp()
{
  for (auto itr = begin(); itr != end(); itr++)
  {
    delete *itr;
  }
}

// add a child
void NaryOp::add(Parse_Tree *child) { _children.push_back(child); }

// iterators for looping through the children
std::vector<Parse_Tree *>::const_iterator NaryOp::begin() const
{
  return _children.begin();
}

std::vector<Parse_Tree *>::const_iterator NaryOp::end() const
{
  return _children.end();
}

//////////////////////////////////////////
// Operator Nodes
//////////////////////////////////////////
EvalResult Program::eval(Ref_Env *env)
{
  EvalResult result;

  // evaluate children and remember the last one
  for (auto itr = begin(); itr != end(); itr++)
  {
    result = (*itr)->eval(env);
  }

  return result;
}

void Program::print(int indent) const
{
  // print the indent
  std::cout << std::setw(indent) << "";
  std::cout << "Program" << std::endl;

  // loop over the children
  for (auto itr = begin(); itr != end(); itr++)
  {
    (*itr)->print(indent + 1);
  }
}

EvalResult Add::eval(Ref_Env *env)
{
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL)
  {
    // real arithmetic
    double x = l.as_real() + r.as_real();
    result.set(x);
  }
  else if (l.type() == STRING and r.type() == STRING)
  {
    std::string x = l.as_string() + r.as_string();
    result.set(x);
  }
  else
  {
    // integer arithmetic
    int x = l.as_integer() + r.as_integer();
    result.set(x);
  }

  return result;
}

void Add::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "+" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Subtract::eval(Ref_Env *env)
{
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL)
  {
    // real arithmetic
    double x = l.as_real() - r.as_real();
    result.set(x);
  }
  else
  {
    // integer arithmetic
    int x = l.as_integer() - r.as_integer();
    result.set(x);
  }
  return result;
}

void Subtract::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "-" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Multiply::eval(Ref_Env *env)
{
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL)
  {
    // real arithmetic
    double x = l.as_real() * r.as_real();
    result.set(x);
  }
  else
  {
    // integer arithmetic
    int x = l.as_integer() * r.as_integer();
    result.set(x);
  }
  return result;
}

void Multiply::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "*" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Divide::eval(Ref_Env *env)
{
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL)
  {
    // real arithmetic
    double x = l.as_real() / r.as_real();
    result.set(x);
  }
  else
  {
    // integer arithmetic
    int x = l.as_integer() / r.as_integer();
    result.set(x);
  }
  return result;
}

void Divide::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "/" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Mod::eval(Ref_Env *env)
{
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // we always return integer
  result.set(l.as_integer() % r.as_integer());

  return result;
}

void Mod::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "MOD" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Power::eval(Ref_Env *env)
{
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // power always returns real
  result.set(pow(l.as_real(), r.as_real()));

  return result;
}

void Power::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "^" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Negation::eval(Ref_Env *env)
{
  EvalResult result;
  EvalResult c = child()->eval(env);

  if (c.type() == REAL)
  {
    result.set(-1 * c.as_real());
  }
  else
  {
    result.set(-1 * c.as_integer());
  }

  return result;
}

void Negation::print(int indent) const
{
  // print myself
  std::cout << std::setw(indent) << "";
  std::cout << "- (NEG)";

  child()->print(indent + 1);
}

Literal::Literal(const Lexer_Token &tok) { this->_tok = tok; }

EvalResult Literal::eval(Ref_Env *env)
{
  EvalResult result;

  if (_tok.tok == INTLIT)
  {
    result.set(stoi(_tok.lexeme));
  }
  else if (_tok.tok == STRLIT)
  {
    result.set(std::string(_tok.lexeme));
  }
  else
  {
    result.set(stof(_tok.lexeme));
  }

  return result;
}

void Literal::print(int indent) const
{
  // print ourself
  std::cout << std::setw(indent) << " ";
  std::cout << _tok.lexeme << std::endl;
}

Variable::Variable(const Lexer_Token &tok) { this->_tok = tok; }

EvalResult Variable::eval(Ref_Env *env)
{
  // retrieve the variable
  return env->get(_tok.lexeme);
}

void Variable::print(int indent) const
{
  // print ourself
  std::cout << std::setw(indent) << "";
  std::cout << _tok.lexeme << std::endl;
}

void Variable::set(Ref_Env *env, EvalResult value)
{
  env->set(_tok.lexeme, value);
}

std::string Variable::name() const { return _tok.lexeme; }

EvalResult Assignment::eval(Ref_Env *env)
{
  EvalResult result;

  Variable *v = (Variable *)left();
  v->set(env, right()->eval(env));

  return result;
}

void Assignment::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "= (assign)" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Display::eval(Ref_Env *env)
{
  EvalResult result;

  // get the value to print
  EvalResult value = child()->eval(env);

  if (value.type() == INTEGER)
  {
    std::cout << value.as_integer() << std::endl;
  }
  else if (value.type() == STRING)
  {
    std::cout << value.as_string() << std::endl;
  }
  else if (value.type() == REAL)
  {
    std::cout << value.as_real() << std::endl;
  }
  else if (value.type() == VECTOR)
  {
    std::vector<int, std::allocator<int>> arrayElements = value.as_array();

    std::cout << "[";
    for (const int &element : arrayElements)
    {
      std::cout << element << ",";
    }
    std::cout << "]" << std::endl;
  }
  return result;
}

void Display::print(int indent) const
{
  // print myself
  std::cout << std::setw(indent) << "";
  std::cout << "DISPLAY" << std::endl;

  child()->print(indent + 1);
}

EvalResult Input::eval(Ref_Env *env)
{
  EvalResult result;
  Variable *v = static_cast<Variable *>(child());
  std::string input;

  // print the prompt and get the input
  std::cout << v->name() << "=";

  // Read the entire line, including spaces
  std::getline(std::cin, input);

  // Check if the input starts and ends with double quotes, treat as string
  if (input.size() >= 2 && input.front() == '"' && input.back() == '"')
  {
    input = input.substr(1, input.size() - 2); // Remove the double quotes
    EvalResult value;
    value.set(input);
    v->set(env, value);
  }
  else
  {
    // Check if the input is numeric
    try
    {
      size_t pos;
      double num = std::stod(input, &pos);

      if (pos == input.size())
      {
        EvalResult value;

        // Check if the numeric value is an integer or has a fractional part
        if (std::floor(num) == num)
        {
          value.set(static_cast<int>(num));
        }
        else
        {
          value.set(num);
        }

        v->set(env, value);
      }
      else
      {
        std::cerr << "Invalid numeric input: " << input << std::endl;
      }
    }
    catch (const std::invalid_argument &e)
    {
      std::cerr << "Invalid input: " << input << std::endl;
    }
  }

  return result;
}

void Input::print(int indent) const
{
  // print myself
  std::cout << std::setw(indent) << "";
  std::cout << "INPUT" << std::endl;

  child()->print(indent + 1);
}

EvalResult Record_Instantiation::eval(Ref_Env *env)
{
  EvalResult result;

  // TODO: Implement this

  return result;
}

void Record_Instantiation::print(int indent) const
{
  // print myself
  std::cout << std::setw(indent) << "";
  std::cout << "NEW";

  child()->print(indent + 1);
}

Record_Declaration::Record_Declaration(const Lexer_Token &_tok)
{
  this->_tok = _tok;
}

std::string Record_Declaration::name()
{
  return _tok.lexeme;
}

EvalResult Record_Declaration::eval(Ref_Env *env)
{
  EvalResult result;

  // TODO: Implement this

  return result;
}

void Record_Declaration::print(int indent) const
{
  // print the indent
  std::cout << std::setw(indent) << "";
  std::cout << "Program" << std::endl;

  // loop over the children
  for (auto itr = begin(); itr != end(); itr++)
  {
    (*itr)->print(indent + 1);
  }
}

EvalResult Branch::eval(Ref_Env *env)
{
  EvalResult result;

  if (left()->eval(env).as_bool())
  {
    right()->eval(env);
  }

  return result;
}

void Branch::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "IF" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Loop::eval(Ref_Env *env)
{
  EvalResult result;

  while (left()->eval(env).as_bool())
  {
    right()->eval(env);
  }

  return result;
}

void Loop::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "WHILE" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Equal::eval(Ref_Env *env)
{
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL)
  {
    // real arithmetic
    bool x = l.as_real() == r.as_real();
    result.set(x);
  }
  else
  {
    // integer arithmetic
    bool x = l.as_integer() == r.as_integer();
    result.set(x);
  }
  return result;
}

void Equal::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "= (Compare)" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Not_Equal::eval(Ref_Env *env)
{
  EvalResult result;

  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  if (l.type() == REAL or r.type() == REAL)
  {
    // real arithmetic
    bool x = l.as_real() != r.as_real();
    result.set(x);
  }
  else
  {
    // integer arithmetic
    bool x = l.as_integer() != r.as_integer();
    result.set(x);
  }

  return result;
}

void Not_Equal::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "<>" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Greater::eval(Ref_Env *env)
{
  EvalResult result;

  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  if (l.type() == REAL or r.type() == REAL)
  {
    // real arithmetic
    bool x = l.as_real() > r.as_real();
    result.set(x);
  }
  else
  {
    // integer arithmetic
    bool x = l.as_integer() > r.as_integer();
    result.set(x);
  }

  return result;
}

void Greater::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << ">" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Less::eval(Ref_Env *env)
{
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL)
  {
    // real arithmetic
    bool x = l.as_real() < r.as_real();
    result.set(x);
  }
  else
  {
    // integer arithmetic
    bool x = l.as_integer() < r.as_integer();
    result.set(x);
  }

  return result;
}

void Less::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "<" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Less_or_Equal::eval(Ref_Env *env)
{
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL)
  {
    // real arithmetic
    bool x = l.as_real() <= r.as_real();
    result.set(x);
  }
  else
  {
    // integer arithmetic
    bool x = l.as_integer() <= r.as_integer();
    result.set(x);
  }

  return result;
}

void Less_or_Equal::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "<=" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Greater_or_Equal::eval(Ref_Env *env)
{
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL)
  {
    // real arithmetic
    bool x = l.as_real() >= r.as_real();
    result.set(x);
  }
  else
  {
    // integer arithmetic
    bool x = l.as_integer() >= r.as_integer();
    result.set(x);
  }

  return result;
}

void Greater_or_Equal::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << ">=" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Record_Access::eval(Ref_Env *env)
{
  EvalResult result;

  // TODO: Implement this

  return result;
}

void Record_Access::print(int indent) const
{
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << ". (Access)" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Parse_List::eval(Ref_Env *env)
{
  return EvalResult();
}

void Parse_List::print(int indent) const
{
  // loop over the children
  for (auto itr = begin(); itr != end(); itr++)
  {
    (*itr)->print(indent + 1);
  }
}

Fun_Def::Fun_Def(const Lexer_Token &tok) : var(tok)
{
}

EvalResult Fun_Def::eval(Ref_Env *env)
{
  EvalResult value;
  value.set(new Closure(this, env));
  env->set(name(), value);
  return EvalResult();
}

std::string Fun_Def::name() const
{
  return var.name();
}

void Fun_Def::print(int indent) const
{
  // print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "fun " << name() << std::endl;
  std::cout << std::setw(indent) << "";
  std::cout << "Parameters";
  left()->print(indent + 1);
  std::cout << std::setw(indent) << "";
  std::cout << "Body";
  right()->print(indent + 1);
}

EvalResult Fun_Call::eval(Ref_Env *env)
{
  // retrieve the function
  EvalResult fr = left()->eval(env);

  // if we don't have a function, return an error
  if (fr.type() != FUNCTION)
  {
    std::cerr << "Error: Attempted to call a non-function" << std::endl;
    return EvalResult();
  }

  // Check parameter binding
  Closure *closure = fr.as_fun();
  Parse_List *params = (Parse_List *)(closure->fun->left());
  Parse_List *args = (Parse_List *)(right());
  if (params->end() - params->begin() != args->end() - args->begin())
  {
    std::cerr << "Error: Incorrect number of arguments" << std::endl;
    return EvalResult();
  }

  // Create a local scope and bind the arguments
  Ref_Env *local = new Ref_Env(closure->env);
  for (auto pitr = params->begin(), aitr = args->begin(); pitr != params->end(); pitr++, aitr++)
  {
    Variable *var = (Variable *)(*pitr);
    Parse_Tree *arg = *aitr;
    local->declare(var->name());     // <-- Forces the parameter to be local
    var->set(local, arg->eval(env)); // <-- Binds the argument
  }

  return closure->fun->right()->eval(local);
}

void Fun_Call::print(int indent) const
{
  std::cout << std::setw(indent) << "";
  std::cout << "function call " << std::endl;
  left()->print(indent + 1);
  right()->print(indent + 1);
}
Array_Declaration::Array_Declaration(const Lexer_Token &type, const Lexer_Token &bound, const Lexer_Token &name)
    : type_(type), bound_(bound), name_(name) {}

EvalResult Array_Declaration::eval(Ref_Env *env)
{
  std::string ref_type = type_.lexeme; // Assuming type_ is a Lexer_Token
  std::string name = name_.lexeme;     // Assuming name_ is a Lexer_Token

  int bounds = 0;

  if (bound_.tok == INTLIT)
  {
    bounds = std::stoi(bound_.lexeme); // If it's an integer literal, use its value
  }
  else if (bound_.tok == ID)
  {
    // If it's an identifier, look up its value in the environment
    EvalResult boundResult = env->get(bound_.lexeme);
    if (boundResult.as_integer())
    {
      bounds = boundResult.as_integer();
    }
    else
    {
      std::cerr << "Error: The array bounds should be an integer or a variable representing an integer." << std::endl;
      return EvalResult(); // Return an error result or throw an exception
    }
  }
  else
  {
    std::cerr << "Error: Unexpected type for array bounds." << std::endl;
    return EvalResult(); // Return an error result or throw an exception
  }

  std::vector<int> arrayValues;

  if (env->lookup(name))
  {
    std::cerr << "Error: Array '" << name << "' is already defined." << std::endl;
    return EvalResult();
  }
  EvalResult result;
  result.set(arrayValues);

  // Assign the array to the environment
  env->set(name, result);

  return EvalResult(); // Return some result if needed
}

void Array_Declaration::print(int indent) const
{
  // Print the array declaration
  std::cout << std::setw(indent) << "";
  std::cout << "Array Declaration" << std::endl;

  // Print type, bound, and name
  std::cout << std::setw(indent + 1) << "";
  std::cout << "Type: " << type_ << std::endl;

  std::cout << std::setw(indent + 1) << "";
  std::cout << "Bound: " << bound_ << std::endl;

  std::cout << std::setw(indent + 1) << "";
  std::cout << "Name: " << name_ << std::endl;
}

EvalResult ArrayAssignment::eval(Ref_Env *env)
{
  EvalResult result;

  // Retrieve the array name from the left side of the assignment
  Variable *arrayVariable = dynamic_cast<Variable *>(left());
  if (arrayVariable)
  {
    EvalResult arrayResult = arrayVariable->eval(env);

    // Check if the arrayVar is an array
    if (arrayResult.type() != EvalType::VECTOR)
    {
      std::cerr << "Error: " << arrayVariable->name() << " is not an array." << std::endl;
      return EvalResult(); // Return an undefined result
    }
    // Get the array name
    std::string arrayName = arrayVariable->name();

    // Retrieve the vector from EvalResult
    std::vector<int> arrayValues = arrayResult.as_array();

    // Push the value to the end of the array
    arrayValues.push_back(right()->eval(env).as_integer());

    // Update the array in the environment
    EvalResult updatedArray;
    updatedArray.set(arrayValues); // Assuming set method is available in your EvalResult class
    env->set(arrayName, updatedArray);
  }
  return result;
}

void ArrayAssignment::print(int indent) const
{
  // Print the array declaration
  std::cout << std::setw(indent) << "";
  std::cout << "Array Assignment" << std::endl;
}

Array_Access::Array_Access(const Lexer_Token &name_array, Lexer_Token &index)
    : name_array(name_array), index_(index)
{
  // Constructor implementation if needed
}

EvalResult Array_Access::eval(Ref_Env *env)
{
  // Retrieve the array name
  std::string arrayName = name_array.lexeme;

  // Check if the array variable exists in the environment
  EvalResult *arrayVar = env->lookup(arrayName);

  EvalResult *arrayval = env->lookup(index_.lexeme);

  int arr_index = 0;

  if (!arrayval)
  {
    arr_index = std::atoi(index_.lexeme.c_str());
  }
  else
  {
    arr_index = arrayval->as_integer();
  }

  // Check if the arrayVar is an array
  if (arrayVar->type() != EvalType::VECTOR)
  {
    std::cerr << "Error: " << arrayName << " is not an array." << std::endl;
    return EvalResult(); // Return an undefined result
  }

  // // Retrieve the vector from EvalResult
  std::vector<int> arrayValues = arrayVar->as_array();

  // // Check if the index is within bounds
  if (arr_index < 0 || arr_index >= arrayValues.size())
  {
    std::cerr << "Error: Index out of bounds for array " << arrayName << std::endl;
    return EvalResult(); // Return an undefined result
  }

  // // Create a new EvalResult object and set its value
  EvalResult result;
  result.set((arrayValues[arr_index]));

  return result;
}

void Array_Access::print(int indent) const
{
  // Print the array declaration
  std::cout << std::setw(indent) << "";
  std::cout << "Array Assignment" << std::endl;
}

Array_Update::Array_Update(const Lexer_Token &name_array, Lexer_Token &index, Lexer_Token &update_value)
    : name_array(name_array), index_(index), update_value_(update_value)
{
}

EvalResult Array_Update::eval(Ref_Env *env)
{
  std::string arrayName = name_array.lexeme;

  EvalResult *arrayVar = env->lookup(arrayName);

  if (arrayVar->type() != EvalType::VECTOR)
  {
    std::cerr << "Error: " << arrayName << " is not an array." << std::endl;
    return EvalResult(); // Return an undefined result
  }

  int arr_index;
  int update_val;

  EvalResult var_val = env->get(index_.lexeme);
  EvalResult val_update = env->get(update_value_.lexeme);

  // Check if the index is a variable
  if (var_val.type() != UNDEFINED)
  {
    arr_index = var_val.as_integer();
  }
  else
  {
    // If the index is not a variable, assume it's a numeric value
    try
    {
      arr_index = std::stoi(index_.lexeme);
    }
    catch (const std::invalid_argument &e)
    {
      std::cerr << "Error: Invalid index value." << std::endl;
      return EvalResult(); // Return an undefined result
    }
  }

  // Check if the update value is a variable
  if (val_update.type() != UNDEFINED)
  {
    update_val = val_update.as_integer();
  }
  else
  {
    // If the update value is not a variable, assume it's a numeric value
    try
    {
      update_val = std::stoi(update_value_.lexeme);
    }
    catch (const std::invalid_argument &e)
    {
      std::cerr << "Error: Invalid update value." << std::endl;
      return EvalResult(); // Return an undefined result
    }
  }

  std::vector<int> arrayValues = arrayVar->as_array();

  // Check if the index is within bounds
  if (arr_index < 0 || arr_index >= arrayValues.size())
  {
    std::cerr << "Error: Index out of bounds for array " << arrayName << std::endl;
    return EvalResult(); // Return an undefined result
  }

  // Update the array value at the specified index
  arrayValues[arr_index] = update_val;

  // Update the array in the environment
  EvalResult updatedArray;
  updatedArray.set(arrayValues); // Assuming set method is available in your EvalResult class
  env->set(arrayName, updatedArray);

  return EvalResult();
}

void Array_Update::print(int indent) const
{
  // Print the array declaration
  std::cout << std::setw(indent) << "";
  std::cout << "Array Updated" << std::endl;
}

Array_Size::Array_Size(const Lexer_Token &name_array)
    : name_array(name_array)
{
  // Constructor implementation if needed
}

EvalResult Array_Size::eval(Ref_Env *env)
{
  // Retrieve the array name
  std::string arrayName = name_array.lexeme;

  // Check if the array variable exists in the environment
  EvalResult *arrayVar = env->lookup(arrayName);

  // Check if the arrayVar is an array
  if (arrayVar->type() != EvalType::VECTOR)
  {
    std::cerr << "Error: " << arrayName << " is not an array." << std::endl;
    return EvalResult(); // Return an undefined result
  }

  // // Retrieve the vector from EvalResult
  std::vector<int> arrayValues = arrayVar->as_array();
  int arr_size = (int)arrayValues.size();

  // // Create a new EvalResult object and set its value
  EvalResult result;
  result.set(arr_size);
  return result;
}

void Array_Size::print(int indent) const
{
  // Print the array declaration
  std::cout << std::setw(indent) << "";
  std::cout << "Array Assignment" << std::endl;
}

struct Employee
{
  std::string name;
  std::string email;
  std::string phone;
  double salary;
};

struct Purchase
{
  std::string itemName;
  double price;
  int quantity;
};

struct Customer
{
  std::string name;
  std::string email;
  std::string phone;
  std::vector<Purchase> purchases;
};

Load_File::Load_File(const Lexer_Token &name_array, const std::string &load_what, std::string &customer_number)
    : name_array(name_array), load_what(load_what), customer_number(customer_number)
{
  // Constructor implementation if needed
}

std::vector<Employee> employees;
std::vector<Customer> customers;
Purchase purchase;

EvalResult Load_File::eval(Ref_Env *env)
{
  employees.clear();
  customers.clear();

  EvalResult var_val = env->get(name_array.lexeme);
  std::string filename = var_val.as_string();
  std::cout << "Opening file: " << filename << std::endl;
  std::fstream infile(filename, std::ios::in);

  if (!infile.is_open())
  {
    std::cerr << "Error opening the file." << std::endl;
    return EvalResult();
  }

  int numEmployees = 0;
  infile >> numEmployees;

  if (infile.fail())
  {
    std::cerr << "Error reading the data." << std::endl;
    return EvalResult(); // Return an error code
  }
  employees.clear(); // Clear the global vector before populating it
  for (int i = 0; i < numEmployees; ++i)
  {
    Employee emp;
    infile.ignore(); // Ignore the newline character after the number of employees
    std::getline(infile, emp.name);
    std::getline(infile, emp.email);
    std::getline(infile, emp.phone);
    infile >> emp.salary;
    employees.push_back(emp);
  }

  int numCustomers = 0;
  infile >> numCustomers;
  infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the newline character after numCustomers

  for (int i = 0; i < numCustomers; ++i)
  {
    Customer cust;
    std::getline(infile >> std::ws, cust.name); // Read and skip leading whitespaces
    std::getline(infile, cust.email);
    std::getline(infile, cust.phone);

    int numPurchases = 0;
    infile >> numPurchases;
    infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the newline character after numPurchases

    for (int j = 0; j < numPurchases; ++j)
    {
      Purchase purchase;
      std::getline(infile >> std::ws, purchase.itemName); // Read and skip leading whitespaces
      infile >> purchase.quantity >> purchase.price;
      infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore the newline character after reading purchase.quantity
      cust.purchases.push_back(purchase);
    }

    customers.push_back(cust);
  }

  if (load_what == "employee")
  {
    std::cout << "Employees:" << std::endl;
    for (const auto &emp : employees)
    {
      std::cout << emp.name << " < " << emp.email << " > "
                << " Phone: " << emp.phone << " Salary: $" << emp.salary << std::endl;
    }
  }
  else if (load_what == "customer")
  {
    std::cout << "\nCustomers:" << std::endl;
    for (int i = 0; i < customers.size(); ++i)
    {
      std::cout << i + 1 << "." << customers[i].name << std::endl;
    }
  }
  else if (load_what == "customer_purchase")
  {

    if (customer_number == "")
    {
      std::cerr << "Invalid Input" << std::endl;
    }
    else
    {
      EvalResult cust_eval = env->get(customer_number);
      int cust_num = cust_eval.as_integer();

      const Customer &selectedCustomer = customers[cust_num - 1];

      std::cout << selectedCustomer.name << " < " << selectedCustomer.email << " > Phone: " << selectedCustomer.phone << std::endl;

      std::cout << "Order History " << std::endl;
      std::cout << "Item                   Price     Quantity     Total" << std::endl;

      for (const auto &purchase : selectedCustomer.purchases)
      {
        std::cout << purchase.itemName << "     $" << purchase.price << "    " << purchase.quantity << "     $" << purchase.quantity * purchase.price << std::endl;
      }
    }
  }

  return EvalResult(); // Return 0 to indicate success
}

void Load_File::print(int indent) const
{
  // Print the array declaration
  std::cout << std::setw(indent) << "";
  std::cout << "File Load" << std::endl;
}

Write_File::Write_File(const Lexer_Token &file_name, const std::string &write_type, const std::string &customer_number, const std::vector<Lexer_Token> &variables)
    : name_array(file_name), write_type(write_type), customer_number(customer_number), variables(variables)
{
  // Additional initialization, if needed
}

EvalResult Write_File::eval(Ref_Env *env)
{
  // Assuming the name_array contains the name of the variable to be written
  EvalResult var_val = env->get(name_array.lexeme);

  std::string filename = var_val.as_string();

  std::cout << "Writing data for " << var_val.as_string() << ":" << std::endl;
  std::cout << "Write Type " << write_type << std::endl;

  int i = 0;
  if (write_type == "employee")
  {
    std::string name = env->get(this->variables[i].lexeme).as_string();
    std::string email = env->get(this->variables[i + 1].lexeme).as_string();
    std::string phone = env->get(this->variables[i + 2].lexeme).as_string();
    double salary = env->get(this->variables[i + 3].lexeme).as_integer();

    std::cout << "salary " << salary << std::endl;

    Employee emp;
    emp.name = name;
    emp.email = email;
    emp.phone = phone;
    emp.salary = salary;

    // Append the new employee to the vector
    employees.push_back(emp);
  }
  else if (write_type == "customer")
  {
    std::string name = env->get(this->variables[i].lexeme).as_string();
    std::string email = env->get(this->variables[i + 1].lexeme).as_string();
    std::string phone = env->get(this->variables[i + 2].lexeme).as_string();

    Customer customer;
    customer.name = name;
    customer.email = email;
    customer.phone = phone;

    // Append the new employee to the vector
    customers.push_back(customer);
  }
  else if (write_type == "customer_purchase")
  {
    if (customer_number == "")
    {
      std::cerr << "Invalid Input" << std::endl;
    }
    else
    {

      std::string item = env->get(this->variables[i].lexeme).as_string();
      int quantity = env->get(this->variables[i + 1].lexeme).as_integer();
      double cost = env->get(this->variables[i + 2].lexeme).as_integer();

      EvalResult cust_eval = env->get(customer_number);
      int cust_num = cust_eval.as_integer();

      Customer &selectedCustomer = customers[cust_num - 1];

      Purchase purchaseDetails;

      purchaseDetails.itemName = item;
      purchaseDetails.quantity = quantity;
      purchaseDetails.price = cost;

      selectedCustomer.purchases.push_back(purchaseDetails);
    }
    // Add other write logic as needed
    return EvalResult();
    }
  return EvalResult(); // Return a placeholder result
}

void Write_File::print(int indent) const
{
}

Close_File::Close_File(const Lexer_Token &file_name)
    : file_name(file_name)
{
}

EvalResult Close_File::eval(Ref_Env *env)
{
  // Assuming the name_array contains the name of the variable to be written
  EvalResult var_val = env->get(file_name.lexeme);

  std::string filename = var_val.as_string();

  std::cout << "Writing data for " << var_val.as_string() << ":" << std::endl;
  int i = 0;
  std::string final_data = "";
  if (employees.size() > 0)
  {
    final_data += std::to_string(employees.size()) + "\n";
    for (const Employee &employee : employees)
    {
      final_data += employee.name + "\n";
      final_data += employee.email + "\n";
      final_data += employee.phone + "\n";
      final_data += std::to_string(employee.salary) + "\n";
    }
  }
  else
  {
    final_data += std::to_string(0) + "\n";
  }

  if (customers.size() > 0)
  {
    final_data += std::to_string(customers.size()) + "\n";
    for (const Customer &customer : customers)
    {
      final_data += customer.name + "\n";
      final_data += customer.email + "\n";
      final_data += customer.phone + "\n";
      const std::vector<Purchase> &purchases = customer.purchases;
      if (purchases.size() > 0)
      {
        final_data += std::to_string(purchases.size()) + "\n";
        for (const Purchase &purchase : purchases)
        {
          final_data += purchase.itemName + "\n";
          final_data += std::to_string(purchase.quantity) + "\n";
          final_data += std::to_string(purchase.price) + "\n";
        }
      }
      else
      {
        final_data += std::to_string(0) + "\n";
      }
    }
  }

  try
  {
    std::ofstream outFile(filename);
    outFile << final_data;
    outFile.close();
  }
  catch (std::exception &e)
  {
    std::cerr << "Failed to write to file: " << e.what() << std::endl;
  }

  // Add other write logic as needed
  exit(0);
  return EvalResult(); // Return a placeholder result
}

void Close_File::print(int indent) const
{
  std::cout << "Closing File" << std::endl;
}
