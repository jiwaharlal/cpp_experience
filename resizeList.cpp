#include <iostream>
#include <list>

struct SatelliteInfo
{
   int azimuth;
   double frequency;
};

int main(int, char**)
{
   std::list<int> intList;
   intList.resize(10);

   std::list<SatelliteInfo> satellites;
   satellites.resize(12);

   std::cout << intList.size() << std::endl;
   std::cout << satellites.size() << std::endl;

   return 0;
}
