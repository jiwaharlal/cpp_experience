#include <iostream>

class A
{
public:
   void print() { std::cout << "Print A" << std::endl; }
};

class B : public A
{
public:
   void print() { std::cout << "Print B" << std::endl; }
};

class C : public A
{
public:
   void print() { std::cout << "Print C" << std::endl; }
   void printMore(int value)
   {
      mValue = value;
      std::cout << "Print more C" << std::endl;
   }
private:
   int mValue;
};

int main()
{
   B b;
   A& a = b;
   static_cast<C&>(a).printMore(10);

   return 0;
}
