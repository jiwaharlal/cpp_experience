#include <iostream>
#include <stdint.h>

int main(int argc, char** argv)
{
   int32_t timestamp = 0x3fffffff;
   std::cout << timestamp << std::endl;

   int64_t longTimestamp = timestamp * 1000;
   std::cout << longTimestamp << std::endl;

   longTimestamp = static_cast<int64_t>(timestamp) * 1000;
   std::cout << longTimestamp << std::endl;

   return 0;
}
