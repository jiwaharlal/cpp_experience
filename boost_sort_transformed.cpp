#include <boost/range/algorithm.hpp>
#include <boost/bind.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/random.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <string>

namespace br = boost::random;

typedef std::pair<size_t, double> tProximityPair;

std::ostream& operator <<(std::ostream& out, const tProximityPair& p)
{
   return out << "<" << p.first << ", " << p.second << ">";
}

int main()
{
   using namespace boost::adaptors;

   std::vector<tProximityPair> pointsProximity;

   static br::mt19937 rng;

   br::uniform_real_distribution<> proximityGen(0, 10);

   for (size_t i = 0; i < 20; ++i)
   {
      pointsProximity.push_back(tProximityPair(i, proximityGen(rng)));
   }

   BOOST_FOREACH(const tProximityPair& p, pointsProximity)
   {
      std::cout << "<" << p.first << ", " << p.second << ">, ";
   }
   std::cout << std::endl << std::endl;

   boost::sort(
         pointsProximity,
         boost::bind(&tProximityPair::second, _1) < boost::bind(&tProximityPair::second, _2));

   struct Closer
   {
      bool operator ()(const tProximityPair& lhs, const tProximityPair& rhs)
      {
         return lhs.second < rhs.second;
      }
   };

   const tProximityPair& closest = *boost::range::min_element(
         pointsProximity,
         boost::bind(&tProximityPair::second, _1) < boost::bind(&tProximityPair::second, _2));

   std::cout << "Closest element is " << closest.first << std::endl;

   BOOST_FOREACH(const tProximityPair& p, pointsProximity)
   {
      std::cout << "<" << p.first << ", " << p.second << ">";
   }

   //boost::copy(pointsProximity, std::ostream_iterator<tProximityPair>(std::cout, ", "));
   //boost::for_each(pointsProximity, boost::bind(&std::ostream::operator <<, &std::cout, _1));
   //boost::for_each(pointsProximity, std::cout << _1);
   std::cout << std::endl;

   return 0;
}
