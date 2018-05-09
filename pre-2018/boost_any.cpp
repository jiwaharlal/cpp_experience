#include <boost/any.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <boost/range/algorithm/copy.hpp>
#include <boost/spirit/home/support/detail/hold_any.hpp>

//std::ostream& operator <<(std::ostream& out, boost::any)
//{

//}

int main()
{
   //std::vector<boost::spirit::hold_any> v;

   //v.push_back(20);
   //v.push_back("hello");

   //boost::copy(v, std::ostream_iterator<boost::any>(std::cout, ", "));

   boost::spirit::hold_any a = std::string("hello");

   std::cout << a << std::endl;

   return 0;
}
