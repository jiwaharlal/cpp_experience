#include <boost/range.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/irange.hpp>
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
   std::vector<int> v;
   boost::copy(boost::irange(0, 10), std::back_inserter(v));

   boost::copy(v, std::ostream_iterator<int>(std::cout, ", "));
   std::cout << std::endl;
}
