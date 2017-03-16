#include <sstream>
#include <string>
#include <iostream>
#include <ios>

int main()
{
   std::stringstream ss;
   ss.width(2);
   ss.fill('0');
   ss << std::hex << std::right;
   ss << 24 << 45 << 87;

   std::cout << '#' << ss.str() << std::endl;

   return 0;
}
