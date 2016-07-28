#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

typedef boost::shared_ptr<int> tIntPtr;
typedef boost::shared_ptr<std::string> tStrPtr;

class IntVisitorBase
{
public:
   virtual void operator ()(int) = 0;
};

class StrVisitorBase
{
public:
   virtual void operator ()(std::string) = 0;
};

class IntStrVisitor: public IntVisitorBase, public StrVisitorBase, public boost::static_visitor<>
{
};

class VisitorChild: public IntStrVisitor
{
public:
   virtual void operator ()(int) { std::cout << "int visited" << std::endl; }
   virtual void operator ()(std::string) { std::cout << "string visited" << std::endl; }
};

int main(int, char**)
{
   boost::variant<int, std::string> v("hello");
   VisitorChild visitor;
   IntStrVisitor* visitorPtr = &visitor;

   visitor(42);

   boost::apply_visitor(*visitorPtr, v);

   return 0;
}
