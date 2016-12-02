#include <boost/phoenix.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <iterator>
#include <boost/range/algorithm.hpp>

int main()
{
   using namespace boost::phoenix::placeholders;

   std::vector<int> v;
   for (int i = 0; i < 15; i++)
   {
      v.push_back(rand() % 100);
   }

   boost::find_if(v, arg1 == 15);

   boost::copy(v, std::ostream_iterator<int>(std::cout, ", "));
   std::cout << std::endl;

   return 0;
}
