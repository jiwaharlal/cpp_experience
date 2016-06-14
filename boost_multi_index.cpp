#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>

using boost::multi_index_container;
using namespace boost::multi_index;

struct RequestInfo
{
   //RequestInfo(int tileId, int requestId, bool isForRoute, bool isIndependent)
      //: this.tileId(tileId), this.requestId(requestId), this.isForRoute(isForRoute), this.isIndependent(isIndependent) {}

   int tileId;
   int requestId;
   bool isForRoute;
   bool isIndependent;
};

struct TileIdComparator
{
   bool operator ()(const RequestInfo& ri, int tileId)
   {
      return ri.tileId < tileId;
   }

   bool operator ()(int tileId, const RequestInfo& ri)
   {
      return tileId < ri.tileId;
   }
};

std::ostream& operator<<(std::ostream& out, const RequestInfo& i)
{
   out << "tileId = " << i.tileId << " requestId = " << i.requestId << " isForRoute "
      << i.isForRoute << " isIndependent " << i.isIndependent;

   return out;
}

struct tileId{};
struct requestId{};

int main(int argc, char** argv)
{

   typedef boost::multi_index_container<
      RequestInfo,
      boost::multi_index::indexed_by<
         boost::multi_index::ordered_unique<boost::multi_index::tag<tileId>, BOOST_MULTI_INDEX_MEMBER(RequestInfo, int, tileId)>,
         boost::multi_index::ordered_unique<boost::multi_index::tag<requestId>, BOOST_MULTI_INDEX_MEMBER(RequestInfo, int, requestId)>
      >
   > tRequestMap;

   tRequestMap requests;

   RequestInfo r1 = {1, 4, true, false};
   RequestInfo r2 = {3, 3, true, true};
   RequestInfo r3 = {2, 5, false, false};

   requests.insert(r1);
   requests.insert(r2);
   requests.insert(r3);

   //tRequestMap::index_const_iterator<tileId> = get<tileId>(
   const typename boost::multi_index::index<tRequestMap, tileId>::type& tileIdMap = get<tileId>(requests);
   const typename boost::multi_index::index<tRequestMap, requestId>::type& reqIdMap = get<requestId>(requests);

   bool isForRoute = tileIdMap.find(1)->isForRoute;
   std::cout << isForRoute << std::endl;

   std::cout << tileIdMap.count(2) << std::endl;
   std::cout << *(tileIdMap.find(3)) << std::endl;

   std::cout << requests.size() << std::endl;
   //requests.erase(tileIdMap.find(1));
   //std::cout << requests.size() << std::endl;

   std::cout << *(tileIdMap.begin()) << std::endl;
   std::cout << *(requests.begin()) << std::endl;

   // on getMapData
   tiles = tileCalculator.calculate();





   std::cout << "===========================" << std::endl;
   std::copy(tileIdMap.begin(), tileIdMap.end(), std::ostream_iterator<const RequestInfo&>(std::cout, "\n"));
   std::cout << "===========================" << std::endl;
   std::copy(reqIdMap.begin(), reqIdMap.end(), std::ostream_iterator<const RequestInfo&>(std::cout, "\n"));

   std::set<int> tilesToDrop = {2, 3};
   tRequestMap rm2;
   std::vector<RequestInfo> rv;
   std::set_difference(tileIdMap.begin(), tileIdMap.end(), tilesToDrop.begin(), tilesToDrop.end(), std::inserter(rm2, rm2.end()), TileIdComparator());

   std::cout << "===========================" << std::endl;
   std::copy(rm2.begin(), rm2.end(), std::ostream_iterator<const RequestInfo&>(std::cout, "\n"));

   return 0;
}
