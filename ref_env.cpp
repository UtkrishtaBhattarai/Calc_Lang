// File: ref_env.cpp
// Purpose: A reference environment class implementation.
#include <map>
#include <string>
#include "parse_tree.h"
#include "ref_env.h"

Ref_Env::Ref_Env() : Ref_Env(nullptr)
{
  // nothing to do here
}

Ref_Env::Ref_Env(Ref_Env *_parent) {
  parent(_parent);
}

// Bind a value to a name
void Ref_Env::set(const std::string &name, const EvalResult &value)
{
  EvalResult *ptr = lookup(name);

  if(ptr != nullptr) {
    //set the value at the right level
    *ptr = value;
  } else {
    //set the value at the local level
    _symbol_table[name] = value;
  }
}


void Ref_Env::declare(const std::string &name)
{
  _symbol_table[name] = EvalResult();  
}


// Retrieve a bound name
EvalResult Ref_Env::get(const std::string &name)
{
  EvalResult *ptr = lookup(name);

  if(ptr != nullptr) {
    return *ptr;
  }

  // if we make it here, we did not find the variable
  EvalResult result;
  result.set_type(UNDEFINED);
  return result;
}


// Access the parent
Ref_Env* Ref_Env::parent()
{
   return _parent; 
}


// Modify the parent
void Ref_Env::parent(Ref_Env *_parent)
{
  this->_parent = _parent;
}

// Find a name's location
EvalResult* Ref_Env::lookup(const std::string &name)
{
  if(_symbol_table.find(name) != _symbol_table.end()) {
    // we have found the varaible
    return &(_symbol_table[name]);
  }

  // check our parent
  if(parent() != nullptr) {
    return parent()->lookup(name);
  }

  // if we make it here, we don't have it
  return nullptr;
}