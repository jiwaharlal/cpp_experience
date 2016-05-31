#include <iostream>
#include <boost/chrono.hpp>
#include <typeinfo>

int main(int, char**)
{
   using namespace boost::chrono;

   std::cout << "At least it builds" << std::endl;
   std::cout << system_clock::now().time_since_epoch() << std::endl;
   std::cout << typeid(system_clock::now().time_since_epoch()).name() << std::endl;

   boost::chrono::microseconds ms(10);
   std::cout << sizeof(ms) << std::endl;

   boost::chrono::microseconds now = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
   std::cout << now << std::endl;
   now += minutes(200);
   int64_t i = now.count();
   std::cout << i << std::endl;
   std::cout << sizeof(const boost::chrono::microseconds&) << std::endl;

   boost::chrono::microseconds minus(-1);
   std::cout << minus << std::endl;

   return 0;
}
