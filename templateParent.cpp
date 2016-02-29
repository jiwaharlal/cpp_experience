#include <iostream>
#include <set>

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
   (void) argc;
   (void) argv;

   Printer<A> pa;

   pa.p();

   return 0;
}

