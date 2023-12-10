// File: parse_tree.h
// Purpose: Class definitions for all of the elements of our parse tree.
#ifndef PARSE_TREE_H
#define PARSE_TREE_H
#include <vector>
#include "lexer.h"
#include "ref_env.h"

// class prototype
class Ref_Env;
class Fun_Def;
class Class_Def;

class Closure
{
public:
  Fun_Def *fun;
  Ref_Env *env;

  Closure(Fun_Def *fun, Ref_Env *env);
};

//////////////////////////////////////////
// Evaluation Results
//////////////////////////////////////////
enum EvalType
{
  VOID,
  INTEGER,
  REAL,
  UNDEFINED,
  BOOLEAN,
  FUNCTION,
  STRING,
  VECTOR
};
class EvalResult
{
public:
  EvalResult();

  // set the type and zero the value
  virtual void set_type(EvalType _type);

  // set the value and infer the type
  virtual void set(int _i);
  virtual void set(double _d);
  virtual void set(bool _b);
  virtual void set(Closure *_fun);
  virtual void set(std::string _b);
  virtual void set(std::vector<int> _myarray);

  // type coercion functions
  virtual int as_integer();
  virtual double as_real();
  virtual bool as_bool();
  virtual Closure *as_fun();
  virtual std::string as_string();
  virtual std::vector<int> as_array();

  // retrieve the type
  virtual EvalType type();

private:
  int _i;                    // an integer
  double _d;                 // a real number
  bool _b;                   // a boolean value
  EvalType _type;            // the type
  Closure *_fun;             // a function definition
  std::string _str;          // a string defination
  std::vector<int> _myarray; // for an array or vector
};

//////////////////////////////////////////
// Pure Virtual Base Class
// (abstract)
//////////////////////////////////////////
class Parse_Tree
{
public:
  virtual ~Parse_Tree();
  virtual EvalResult eval(Ref_Env *env) = 0;
  virtual void print(int indent) const = 0; // <- =0 syntax indicates pure virtual
};

//////////////////////////////////////////
// Base classes for the operators
//////////////////////////////////////////
class UnaryOp : public Parse_Tree
{
public:
  // destructor
  virtual ~UnaryOp();

  // accessor and mutator for the child
  virtual Parse_Tree *child() const;
  virtual void child(Parse_Tree *_child);

private:
  Parse_Tree *_child;
};

class BinaryOp : public Parse_Tree
{
public:
  // destructor
  virtual ~BinaryOp();

  // accessor and mutator for the children
  virtual Parse_Tree *left() const;
  virtual void left(Parse_Tree *_left);
  virtual Parse_Tree *right() const;
  virtual void right(Parse_Tree *_right);

private:
  Parse_Tree *_left;
  Parse_Tree *_right;
};

class NaryOp : public Parse_Tree
{
public:
  // destructor
  virtual ~NaryOp();

  // add a child
  void add(Parse_Tree *child);

  // iterators for looping through the children
  virtual std::vector<Parse_Tree *>::const_iterator begin() const;
  virtual std::vector<Parse_Tree *>::const_iterator end() const;

private:
  std::vector<Parse_Tree *> _children;
};

//////////////////////////////////////////
// Operator Nodes
//////////////////////////////////////////
class Program : public NaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Add : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Subtract : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Multiply : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Divide : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Mod : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Power : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Negation : public UnaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Literal : public Parse_Tree
{
public:
  Literal(const Lexer_Token &tok);

  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;

private:
  Lexer_Token _tok;
};

class Variable : public Parse_Tree
{
public:
  Variable(const Lexer_Token &tok);

  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;

  virtual void set(Ref_Env *env, EvalResult value);
  virtual std::string name() const;

private:
  Lexer_Token _tok;
};

class Assignment : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Display : public UnaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Input : public UnaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Record_Instantiation : public UnaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Record_Declaration : public NaryOp
{
public:
  Record_Declaration(const Lexer_Token &_tok);
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
  virtual std::string name();

private:
  Lexer_Token _tok;
};

class Branch : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Loop : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Equal : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Not_Equal : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Greater : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Less : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Less_or_Equal : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Greater_or_Equal : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Record_Access : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Parse_List : public NaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Fun_Def : public BinaryOp
{
public:
  Fun_Def(const Lexer_Token &tok);
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
  virtual std::string name() const;

private:
  Variable var;
};

class Fun_Call : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  virtual void print(int indent) const;
};

class Array_Declaration : public Parse_Tree
{
public:
  Array_Declaration(const Lexer_Token &type, const Lexer_Token &bound, const Lexer_Token &name);
  EvalResult eval(Ref_Env *env) override;
  void print(int indent) const override;

private:
  Lexer_Token type_;
  Lexer_Token bound_;
  Lexer_Token name_;
};


class ArrayAssignment : public BinaryOp
{
public:
  virtual EvalResult eval(Ref_Env *env);
  void print(int indent) const override;
};

class Array_Access : public Parse_Tree {
public:
    Array_Access(const Lexer_Token& name_array, Lexer_Token &index);
    virtual EvalResult eval(Ref_Env* env) override;
    void print(int indent) const override;

private:
    Lexer_Token name_array;
    Lexer_Token index_;
};


class Array_Update : public Parse_Tree
{
public:
  Array_Update(const Lexer_Token &name_array, Lexer_Token &index, Lexer_Token &update_value);
  virtual EvalResult eval(Ref_Env *env) override;
  void print(int indent) const override;

private:
  Lexer_Token name_array;
  Lexer_Token index_;
  Lexer_Token update_value_;
};

class Array_Size : public Parse_Tree
{
public:
  Array_Size(const Lexer_Token &name_array);
  virtual EvalResult eval(Ref_Env *env) override;
  void print(int indent) const override;

private:
  Lexer_Token name_array;
};



class Load_File : public Parse_Tree
{
public:
  Load_File(const Lexer_Token &name_array, const std::string &load_what, std::string &customer_number);
  virtual EvalResult eval(Ref_Env *env) override;
  void print(int indent) const override;

private:
  Lexer_Token name_array;
  std::string load_what;
  std::string customer_number;
};

class Write_File : public Parse_Tree
{
public:
    Write_File(const Lexer_Token &name_array, const std::string &write_type, const std::string &customer_number, const std::vector<Lexer_Token> &variables);
    virtual EvalResult eval(Ref_Env *env) override;
    void print(int indent) const override;

private:
    Lexer_Token name_array;
    std::string write_type;
    std::string customer_number;
    std::vector<Lexer_Token> variables;
};



class Close_File : public Parse_Tree
{
public:
  Close_File(const Lexer_Token &file_name);
  virtual EvalResult eval(Ref_Env *env) override;
  void print(int indent) const override;

private:
  Lexer_Token file_name;
};


class Class_Declaration : public Parse_Tree
{
public:
  Class_Declaration(const Lexer_Token &name);
  EvalResult eval(Ref_Env *env) override;
  void print(int indent) const override;

private:
  Lexer_Token name_;
};


#endif
