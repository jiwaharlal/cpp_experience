#include <boost/lexical_cast.hpp>
#include <vector>
#include <iostream>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>


int main()
{
   std::vector<int> ints;
   //boost::copy(boost::irange()
   using boost::algorithm::join;
   using boost::adaptors::transform;
   std::string intSequence = join(transform(boost::irange(1, 10), &boost::lexical_cast<std::string, int>), ", ");

   std::cout << intSequence << std::endl;

   return 0;
}
