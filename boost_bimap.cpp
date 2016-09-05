#include <iostream>
#include <boost/bimap.hpp>
#include <iterator>
#include <algorithm>
#include <string>
#include <boost/lambda/lambda.hpp>
#include <typeinfo>
#include <utility>
#include <boost/range/adaptors.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>

typedef boost::bimap<int, std::string> tIntStringMap;

template <typename FirstType, typename SecondType>
void printTypes(const boost::bimap<FirstType, SecondType>& bm)
{
   std::cout << "First: " << typeid(bm.left).name() << std::endl;
}

template <class T1>
struct getFirst
{
   template <typename PairType>
   const T1& operator ()(const PairType& p)
   {
      return p.first;
   }
};

struct getSecond
{
   std::string operator ()(tIntStringMap::left_const_reference p)
   {
      return p.second;
   }
};

int main(int, char**)
{
   tIntStringMap numberMap;

   numberMap.insert(tIntStringMap::value_type(1, "one"));
   numberMap.insert(tIntStringMap::value_type(2, "two"));
   numberMap.insert(tIntStringMap::value_type(3, "three"));

   std::vector<int> v;

   std::string optionsStr = boost::algorithm::join(boost::adaptors::values(numberMap.left), ", ");
   std::cout << optionsStr << std::endl;

   return 0;
}
