#include <iostream>
#include <limits>

int main(int argc, char** argv)
{
   (void) argc;
   (void) argv;

   //int minLon = -(1 << 31);
   int minLon = std::numeric_limits<int>::min();
   //int maxLon = (1 << 30) - 1;
   int maxLon = std::numeric_limits<int>::max();

   std::cout << "Longitude min: " << minLon << " max: " << maxLon << std::endl;

   int minLat = -(1 << 30 );
   int maxLat = 1 << 30;

   std::cout << "Latitude min: " << minLat << " max: " << maxLat << std::endl;

   return 0;
}
