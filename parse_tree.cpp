// File: parse_tree.cpp
// Purpose: Implementation of the parse tree classes
#include "parse_tree.h"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

//////////////////////////////////////////
// Evaluation Results
//////////////////////////////////////////
EvalResult::EvalResult() { this->_type = VOID; }

void EvalResult::set_type(EvalType _type) {
  this->_type = _type;
  this->_i = 0;
  this->_d = 0;
}

// set the value and infer the type
void EvalResult::set(int _i) {
  this->_i = _i;
  _type = INTEGER;
}

void EvalResult::set(double _d) {
  this->_d = _d;
  _type = REAL;
}

void EvalResult::set(std::string _str)
{
  this-> _str = _str;
  _type = STRING;
}

void EvalResult::set(std::vector<MyVariant> _myvector)
{
  this->_myVector = _myVector;
  _type = LIST;
}


void EvalResult::set(bool _b) {
  this->_b = _b;
  _type = BOOLEAN;
}

void EvalResult::set(Fun_Def *_fun) {
  this->_fun = _fun;
  _type = FUNCTION;
}

// type coercion functions
int EvalResult::as_integer() {
  if (_type == INTEGER) {
    return _i;
  } else {
    return (int)_d;
  }
}

double EvalResult::as_real() {
  if (_type == REAL) {
    return _d;
  } else {
    return (double)_i;
  }
}


bool EvalResult::as_bool() {
  return _b;
}

Fun_Def *EvalResult::as_fun() {
  return _fun;
}

std::string EvalResult::as_string()
{
  if(_type == STRING)
  {
    return std::string(_str);
  }
  else
  {
    return _str;
  }
  
}

using MyVariant = std::variant<int, double, std::string>;

std::vector<MyVariant> EvalResult::as_vector()
{
  if (_type == LIST)
  {
    return _myVector;
  }
  return _myVector;
}

// retrieve the type
EvalType EvalResult::type() { return _type; }

//////////////////////////////////////////
// Base classes for the operators
//////////////////////////////////////////
Parse_Tree::~Parse_Tree() {
  // nothing to do
}

// destructor
UnaryOp::~UnaryOp() { delete child(); }

// accessor and mutator for the child
Parse_Tree *UnaryOp::child() const { return _child; }

void UnaryOp::child(Parse_Tree *_child) { this->_child = _child; }

BinaryOp::~BinaryOp() {
  delete left();
  delete right();
}

// accessor and mutator for the children
Parse_Tree *BinaryOp::left() const { return _left; }

void BinaryOp::left(Parse_Tree *_left) { this->_left = _left; }

Parse_Tree *BinaryOp::right() const { return _right; }

void BinaryOp::right(Parse_Tree *_right) { this->_right = _right; }

NaryOp::~NaryOp() {
  for (auto itr = begin(); itr != end(); itr++) {
    delete *itr;
  }
}

// add a child
void NaryOp::add(Parse_Tree *child) { _children.push_back(child); }

// iterators for looping through the children
std::vector<Parse_Tree *>::const_iterator NaryOp::begin() const {
  return _children.begin();
}

std::vector<Parse_Tree *>::const_iterator NaryOp::end() const {
  return _children.end();
}

//////////////////////////////////////////
// Operator Nodes
//////////////////////////////////////////
EvalResult Program::eval(Ref_Env *env) {
  EvalResult result;

  // evaluate children and remember the last one
  for (auto itr = begin(); itr != end(); itr++) {
    result = (*itr)->eval(env);
  }

  return result;
}

void Program::print(int indent) const {
  // print the indent
  std::cout << std::setw(indent) << "";
  std::cout << "Program" << std::endl;

  // loop over the children
  for (auto itr = begin(); itr != end(); itr++) {
    (*itr)->print(indent + 1);
  }
}

EvalResult Add::eval(Ref_Env *env) {
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL) {
    // real arithmetic
    double x = l.as_real() + r.as_real();
    result.set(x);
  }
  if(l.type() == STRING and r.type()== STRING)
  {
    std::string x = l.as_string() + r.as_string();
    result.set(x);
  }
   else {
    // integer arithmetic
    int x = l.as_integer() + r.as_integer();
    result.set(x);
  }

  return result;
}

void Add::print(int indent) const {
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "+" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Subtract::eval(Ref_Env *env) {
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL) {
    // real arithmetic
    double x = l.as_real() - r.as_real();
    result.set(x);
  } else {
    // integer arithmetic
    int x = l.as_integer() - r.as_integer();
    result.set(x);
  }
  return result;
}

void Subtract::print(int indent) const {
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "-" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Multiply::eval(Ref_Env *env) {
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL) {
    // real arithmetic
    double x = l.as_real() * r.as_real();
    result.set(x);
  } else {
    // integer arithmetic
    int x = l.as_integer() * r.as_integer();
    result.set(x);
  }
  return result;
}

void Multiply::print(int indent) const {
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "*" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Divide::eval(Ref_Env *env) {
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL) {
    // real arithmetic
    double x = l.as_real() / r.as_real();
    result.set(x);
  } else {
    // integer arithmetic
    int x = l.as_integer() / r.as_integer();
    result.set(x);
  }
  return result;
}

void Divide::print(int indent) const {
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "/" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Mod::eval(Ref_Env *env) {
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // we always return integer
  result.set(l.as_integer() % r.as_integer());

  return result;
}

void Mod::print(int indent) const {
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "MOD" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Power::eval(Ref_Env *env) {
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // power always returns real
  result.set(pow(l.as_real(), r.as_real()));

  return result;
}

void Power::print(int indent) const {
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "^" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Negation::eval(Ref_Env *env) {
  EvalResult result;
  EvalResult c = child()->eval(env);

  if (c.type() == REAL) {
    result.set(-1 * c.as_real());
  } else {
    result.set(-1 * c.as_integer());
  }

  return result;
}

void Negation::print(int indent) const {
  // print myself
  std::cout << std::setw(indent) << "";
  std::cout << "- (NEG)";

  child()->print(indent + 1);
}

Literal::Literal(const Lexer_Token &tok) { this->_tok = tok; }

EvalResult Literal::eval(Ref_Env *env) {
  EvalResult result;

  if (_tok.tok == INTLIT) {
    result.set(stoi(_tok.lexeme));
  }
  else if(_tok.tok == STRLIT)
  {
    result.set(_tok.lexeme);
  }


  else if(_tok.tok == VECT_LITERAL)
  {
    result.set(_tok.lexeme);
  }
   else {
    result.set(stof(_tok.lexeme));
  }

  return result;
}

void Literal::print(int indent) const {
  // print ourself
  std::cout << std::setw(indent) << "";
  std::cout << _tok.lexeme << std::endl;
}

Variable::Variable(const Lexer_Token &tok) { this->_tok = tok; }

EvalResult Variable::eval(Ref_Env *env) {
  // retrieve the variable
  return env->get(_tok.lexeme);
}

void Variable::print(int indent) const {
  // print ourself
  std::cout << std::setw(indent) << "";
  std::cout << _tok.lexeme << std::endl;
}

void Variable::set(Ref_Env *env, EvalResult value) {
  env->set(_tok.lexeme, value);
}

std::string Variable::name() const { return _tok.lexeme; }

EvalResult Assignment::eval(Ref_Env *env) {
  EvalResult result;

  Variable *v = (Variable *)left();
  v->set(env, right()->eval(env));

  return result;
}

void Assignment::print(int indent) const {
  // print the right child
  right()->print(indent + 1);

  // indent and print ourself
  std::cout << std::setw(indent) << "";
  std::cout << "= (assign)" << std::endl;

  // print the left child
  left()->print(indent + 1);
}

EvalResult Display::eval(Ref_Env *env) {
  EvalResult result;

  // get the value to print
  EvalResult value = child()->eval(env);

  if (value.type() == INTEGER) {
    std::cout << value.as_integer() << std::endl;
  } else if (value.type() == REAL) {
    std::cout << value.as_real() << std::endl;
  }
  else if(value.type() == STRING)
  {
    std::cout << value.as_string() << std::endl;
  }
  else if(value.type() == LIST)
  {
    std:: cout << value.type()  << std::endl;
  }

  return result;
}

void Display::print(int indent) const {
  // print myself
  std::cout << std::setw(indent) << "";
  std::cout << "DISPLAY" << std::endl;

  child()->print(indent + 1);
}

EvalResult Input::eval(Ref_Env *env) {
  EvalResult result;
  Variable *v = (Variable*) child();
  double num;

  // print the prompt and get the number
  std::cout << v->name() << "=";
  std::cin >> num;

  // build the value to the smallest type 
  EvalResult value;
  if(num - (int) num == 0.0) {
    value.set((int) num);
  } else {
    value.set(num);
  }

  // bind the variable
  v->set(env, value);

  return result;
}

void Input::print(int indent) const {
  // print myself
  std::cout << std::setw(indent) << "";
  std::cout << "INPUT" << std::endl;

  child()->print(indent + 1);
}



EvalResult Record_Instantiation::eval(Ref_Env *env) {
  EvalResult result;

  //TODO: Implement this

  return result;
}


void Record_Instantiation::print(int indent) const {
  // print myself
  std::cout << std::setw(indent) << "";
  std::cout << "NEW";

  child()->print(indent + 1);
}


Record_Declaration::Record_Declaration(const Lexer_Token &_tok) {
  this->_tok = _tok;
}


std::string Record_Declaration::name() {
  return _tok.lexeme;
}

EvalResult Record_Declaration::eval(Ref_Env *env) {
  EvalResult result;

  //TODO: Implement this

  return result;
}


void Record_Declaration::print(int indent) const
{
  // print the indent
  std::cout << std::setw(indent) << "";
  std::cout << "Program" << std::endl;

  // loop over the children
  for (auto itr = begin(); itr != end(); itr++) {
    (*itr)->print(indent + 1);
  }
}


EvalResult Branch::eval(Ref_Env *env) {
  EvalResult result;

  if(left()->eval(env).as_bool()) {
    result = right()->eval(env);
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


EvalResult Loop::eval(Ref_Env *env) {
  EvalResult result;

  while(left()->eval(env).as_bool()) {
    result = right()->eval(env);
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


EvalResult Equal::eval(Ref_Env *env) {
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL) {
    // real arithmetic
    bool x = l.as_real() == r.as_real();
    result.set(x);
  } else {
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


EvalResult Not_Equal::eval(Ref_Env *env) {
  EvalResult result;

  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  if (l.type() == REAL or r.type() == REAL) {
    // real arithmetic
    bool x = l.as_real() != r.as_real();
    result.set(x);
  } else {
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


EvalResult Greater::eval(Ref_Env *env) {
  EvalResult result;
  
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  if (l.type() == REAL or r.type() == REAL) {
    // real arithmetic
    bool x = l.as_real() > r.as_real();
    result.set(x);
  } else {
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


EvalResult Less::eval(Ref_Env *env) {
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL) {
    // real arithmetic
    bool x = l.as_real() < r.as_real();
    result.set(x);
  } else {
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


EvalResult Less_or_Equal::eval(Ref_Env *env) {
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL) {
    // real arithmetic
    bool x = l.as_real() <= r.as_real();
    result.set(x);
  } else {
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


EvalResult Greater_or_Equal::eval(Ref_Env *env) {
  EvalResult result;
  EvalResult l = left()->eval(env);
  EvalResult r = right()->eval(env);

  // handle our types (TODO: This will become more complicated later)
  if (l.type() == REAL or r.type() == REAL) {
    // real arithmetic
    bool x = l.as_real() >= r.as_real();
    result.set(x);
  } else {
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


EvalResult Record_Access::eval(Ref_Env *env) {
  EvalResult result;

  //TODO: Implement this

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
  for (auto itr = begin(); itr != end(); itr++) {
    (*itr)->print(indent + 1);
  }
}


Fun_Def::Fun_Def (const Lexer_Token &tok) : var(tok)
{
}


EvalResult Fun_Def::eval(Ref_Env *env)
{
  // build a container to store ourselves in the environment
  EvalResult fun;
  fun.set(this);

  //insert ourselves into the environment
  var.set(env, fun);
  
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
  left()->print(indent+1);
  std::cout << std::setw(indent) << "";
  std::cout << "Body";
  right()->print(indent+1);
}


EvalResult Fun_Call::eval(Ref_Env *env)
{
  Variable *fref = (Variable *) left(); 
  EvalResult fun = fref->eval(env);

  // check to see if it's a function
  if(fun.type() != FUNCTION) {
    std::cerr << "Error: " << fref->name() << " is not a function." << std::endl;
    return EvalResult();
  }

  // create a local scope for the function
  Ref_Env *local_env = new Ref_Env(env);

  // bind the parameters to the local scope
  Parse_List *params = (Parse_List *) (fun.as_fun()->left());
  Parse_List *args = (Parse_List*) right();
  int param_count = params->end() - params->begin();
  int arg_count = args->end() - args->begin();
  if(param_count > arg_count) { 
    std::cerr << "Error: Too few arguments for function " << fref->name() << std::endl;
    return EvalResult();
  } else if(param_count < arg_count) {
    std::cerr << "Error: Too many arguments for function " << fref->name() << std::endl;
    return EvalResult();
  }

  for(auto pitr = params->begin(), aitr = args->begin(); pitr != params->end(); pitr++, aitr++) {
    Variable *v = (Variable *) *pitr;
    local_env->declare(v->name());  // force parameters to be local
    local_env->set(v->name(), (*aitr)->eval(env));
  }

  //evaluate the function and return the result
  EvalResult result = fun.as_fun()->right()->eval(local_env); 

  // destroy the local environment
  delete local_env;

  return result;
}


void Fun_Call::print(int indent) const
{
  std::cout << std::setw(indent) << "";
  std::cout << "function call " << std::endl;
  left()->print(indent+1);
  right()->print(indent+1);
}