#include <iostream>
#include <string>

int main(int, char**)
{
   int speed = 300;
   std::cout << (speed ?: 200) << std::endl;
   //std::cout << std::endl;

   return 0;
}
