#include <iostream>
#include <vector>
#include <boost/assign.hpp>
#include <iterator>
#include <boost/range/algorithm.hpp>
#include <sstream>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filtering_stream.hpp>

namespace io = boost::iostreams;

using namespace boost::assign;

int main()
{
   std::stringstream ss;
   std::string helloStr("hello");
   helloStr[3] = static_cast<char>(0);
   std::cout << helloStr << " : " << helloStr.size() << std::endl;

   std::vector<char> chars;
   boost::copy(helloStr, std::back_inserter(chars));

   io::filtering_ostream out(io::back_inserter(chars));
   out << " world";
   out.flush();

   boost::copy(chars, std::ostream_iterator<char>(std::cout, ", "));
   std::cout << std::endl;

   return 0;
}
