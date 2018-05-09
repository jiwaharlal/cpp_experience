#include <iostream>
#include <list>
#include <vector>

struct SatelliteInfo
{
   int azimuth;
   double frequency;
};

int main(int, char**)
{
    std::vector<int> v;
    std::cout << "Max size: " << v.max_size() << std::endl;

   std::list<int> intList;
   intList.resize(10);

   std::list<SatelliteInfo> satellites;
   satellites.resize(12);

   std::cout << intList.size() << std::endl;
   std::cout << satellites.size() << std::endl;

   return 0;
}
