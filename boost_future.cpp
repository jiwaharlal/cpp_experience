#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

int main(int, char**)
{
   typedef boost::shared_ptr<std::string> tStrPtr;
   tStrPtr str1(new std::string("hello"));
   tStrPtr str2 = str1;

   std::cout << (str1 == str2) << std::endl;

   return 0;
}
