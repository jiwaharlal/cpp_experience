#include <iostream>
#include <iterator>
#include <algorithm>

union Wheels
{
   //Wheels()
      //: rearLeft(0), rearRight(0), frontLeft(0), frontRight(0)
   //{}

   struct 
   {
      int rearLeft;
      int rearRight;
      int frontLeft;
      int frontRight;
   };
   int wheels[4];
};

std::ostream& operator <<(std::ostream& out, const Wheels& w)
{
   std::copy(w.wheels, w.wheels + 4, std::ostream_iterator<int>(out, " "));
   return out;
}

int main(int argc, char** argv)
{
   Wheels w = {};

   w.rearLeft = 34;
   w.frontRight = 87;

   std::cout << sizeof(w) << std::endl;
   std::cout << w << std::endl;
   return 0;
}
