#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <iostream>

int main()
{
   boost::optional<int> i; //= 5;
   std::cout << std::boolalpha << i.is_initialized() << std::endl;

   std::cout << i << std::endl;
   //std::cout << i.get() << std::endl;

   return 0;
}
