#include <iostream>
#include <boost/function.hpp>
#include <boost/bind.hpp>

struct ActionType
{
   enum EType
   {
      OPEN_MAP_FILE,
      PRINT_STRING
   };
};

class IActionSet
{
   std::list<IAction*> getActions() = 0;
   void subscribeOnUpdates(IActionUpdateListener*);
};

class IAction
{
   virtual bool isEnabled();
   virtual bool call();
   virtual ActionType::EType type();
};

IActionSet& CNavigationEditor::getActionSet();
IActionSet& CMapLayerController::getActionSet();

class CActionBase;

struct IArgs
{
   //virtual void setTo(CActionBase&) = 0;
   virtual void call(IAction&)
   {
      IAction.call(&this);
   }
};

template <typename ArgType1>
struct TSingleArg: public IArgs
{
   TSingleArg(ArgType1 arg1) : mArg1(arg1) {}

   ArgType1& mArg1;
};

template <typename T>
TSingleArg<T> singleArg(T& arg)
{
   return TSingleArg<T>(arg);
};

template <typename ArgType1>
struct TArgs1: public IArgs
{
};

class IAction
{
public:
   IAction(ActionType::EType type) : mType(type) {}

   virtual bool call(IArgs&) = 0;

   virtual ActionType::EType type() { return mType; }

private:
   ActionType::EType mType;
};

template <typename ArgType1>
class TSingleArgAction: public IAction
{
public:
   TSingleArgAction(ActionType::EType type, boost::function<void(ArgType1)>& function)
      : IAction(type)
      , mFunction(function)
   {}

   virtual bool call(IArgs&)
   {
      std::cout << "Action of type " << type() << " was called without arguments" << std::endl;
      return false;
   }

   virtual bool call(const TSingleArg<ArgType1>& args)
   {
      mFunction(args.mArg1);
      return true;
   }

private:
   boost::function<void(ArgType1)> mFunction;
};

class Printer
{
public:
   void printHello()
   {
      std::cout << "Hello" << std::endl;
   }

   void printString(const std::string& str)
   {
      std::cout << "Print string: " << str << std::endl;
   }
};

//class CPrintAction: public TSingleArgAction<std::string>
//{
//public:
   //CPrintAction()
   //{
   //}
   //virtual ActionType::EType type() { return ActionType::PRINT_STRING; }
//};

int main()
{
   std::cout << "action hello" << std::endl;
   Printer p;
   boost::function<void(std::string)> f = boost::bind(&Printer::printString, &p, _1);

   f(std::string("hello"));

   //boost::function<void(std::string)> f = boost::bind(&Printer::printString, &p);
   IAction* action = new TSingleArgAction<std::string>(ActionType::PRINT_STRING, f);

   TSingleArg<std::string> args("hello");
   action->call(args);
}
