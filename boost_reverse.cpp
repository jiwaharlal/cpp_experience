#include <boost/foreach.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm.hpp>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <set>
#include <vector>

int main()
{
   std::set<int> ints;
   for (int i = 0; i < 20; ++i)
   {
      ints.insert(rand() % 100);
   }

   boost::copy(ints, std::ostream_iterator<int>(std::cout, ", "));
   std::cout << std::endl;

   boost::copy(boost::adaptors::reverse(ints), std::ostream_iterator<int>(std::cout, ", "));
   std::cout << std::endl;

   return 0;
}
