#include <boost/assign/list_of.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <iostream>
#include <map>

int main()
{
   std::map<int, std::string> m = boost::assign::map_list_of
      (1, "one")
      (2, "two")
      (3, "three")
      (4, "four");

   m.erase(m.begin(), m.find(3));

   boost::copy(boost::adaptors::values(m), std::ostream_iterator<std::string>(std::cout, ", "));
   std::cout << std::endl;

   return 0;
}
