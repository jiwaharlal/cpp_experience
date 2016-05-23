#include <iostream>
#include <stdint.h>

int main(int argc, char** argv)
{
   std::cout << "Sizeof 34: " << sizeof(34) << std::endl;
   std::cout << "Sizeof 34LL: " << sizeof(34LL) << std::endl;
   std::cout << "Sizeof uintmax_t: " << sizeof(uintmax_t) << std::endl;

   int32_t timestamp = 0x3fffffff;
   std::cout << timestamp << std::endl;

   int64_t longTimestamp = timestamp * 1000;
   std::cout << longTimestamp << std::endl;

   longTimestamp = static_cast<int64_t>(timestamp) * 1000;
   std::cout << longTimestamp << std::endl;

   return 0;
}
