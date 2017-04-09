#ifndef FUNCTION_MAPPER_H
#define FUNCTION_MAPPER_H

#include <string>
#include <map>
#include <functional>
#include <memory>
#include "shape.h"

using namespace std;

/* Mappeur "string"=>ptr_function */
class BaseCommand
{
public:
    virtual ~BaseCommand() {}
};

template <class... ArgTypes>
class Command : public BaseCommand
{
  typedef std::function<void(ArgTypes...)> FuncType;
  FuncType f_;
  public:
    Command() {}
    Command(FuncType f) : f_(f) {}
    void operator()(ArgTypes... args) { if(f_) f_(args...); }
};

class CommandManager
{
  typedef shared_ptr<BaseCommand> BaseCommandPtr;
  typedef map<string, BaseCommandPtr> FMap;
  public :

  template <class T>
  void add(string name, const T& cmd)
  {
     fmap1.insert(pair<string, BaseCommandPtr>(name, BaseCommandPtr(new T(cmd))));
  }

  FMap::iterator find(string name){
  	return fmap1.find(name);
  }

  FMap::iterator end(){
  	return fmap1.end();
  }

  template <class... ArgTypes>
  void execute(string name, ArgTypes... args)
  {
    typedef Command<ArgTypes...> CommandType;
    FMap::const_iterator it = fmap1.find(name);
    if(it != fmap1.end()){
      CommandType* c = dynamic_cast<CommandType*>(it->second.get());
      if(c){
    	(*c)(args...);
      }
    }
    else
    	printf("Unknown function \"%s\"\n", name.c_str());
  }

  private :
    FMap fmap1;
};

#endif // FUNCTION_MAPPER_H