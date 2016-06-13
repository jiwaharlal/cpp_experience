#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <iostream>

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

   requests.insert(r1);

   //tRequestMap::index_const_iterator<tileId> = get<tileId>(
   const typename boost::multi_index::index<tRequestMap, tileId>::type& tileIdMap = get<tileId>(requests);

   bool isForRoute = tileIdMap.find(1)->isForRoute;
   std::cout << isForRoute << std::endl;

   std::cout << tileIdMap.count(2) << std::endl;
   std::cout << *(tileIdMap.find(3)) << std::endl;

   std::cout << requests.size() << std::endl;
   requests.erase(tileIdMap.find(1));
   std::cout << requests.size() << std::endl;

   return 0;
}
