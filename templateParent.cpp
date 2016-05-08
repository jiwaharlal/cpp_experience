#include <iostream>
#include <set>
#include <boost/static_assert.hpp>
#include <boost/assert.hpp>

class A
{
public:
   void print() {
      std::cout << "A::print" << std::endl;
   }
};

class B
{
public:
   typedef typename std::set<int>::const_iterator IteratorType;
   void print() {
      std::cout << "B::print" << std::endl;
   }
};

template <class ParentType>
class Printer: public ParentType
{
public:
   void p() {
      ParentType::print();
   }
};

int main(int argc, char** argv) {
   //BOOST_STATIC_ASSERT(sizeof(double) == 8);
   (void) argc;
   (void) argv;

   std::cout << "external " << EXTERNAL_STR << std::endl;

   Printer<A> pa;

   pa.p();

   //BOOST_ASSERT(false);

   return 0;
}

