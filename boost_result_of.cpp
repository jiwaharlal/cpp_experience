#include <iostream>
#include <boost/utility/result_of.hpp>
#include <boost/type_index.hpp>

struct strLen
{
   int operator ()(int)
   {
      return 2;
   }
};

int main()
{
   typedef boost::result_of<strLen>::type tResultType;
   std::cout << boost::typeindex::type_id<tResultType>().pretty_name() << "\n";

   return 0;
}
