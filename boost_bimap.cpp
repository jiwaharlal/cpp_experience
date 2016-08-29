#include <iostream>
#include <boost/bimap.hpp>
#include <iterator>
#include <algorithm>
#include <string>
#include <boost/lambda/lambda.hpp>
#include <typeinfo>
#include <utility>
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

   //boost::transform(numberMap.left, std::back_inserter(v), [](tIntStringMap::left_const_reference v) {return v.first;});
   boost::transform(numberMap.left, std::ostream_iterator<int>(std::cout, ", "), getFirst<int>());
   std::cout << std::endl;
   boost::transform(numberMap.left, std::ostream_iterator<std::string>(std::cout, ", "), getSecond());
   std::cout << std::endl;

   //tIntStringMap::
   //tIntStringMap::left_iterator it = numberMap.begin();
   //numberMap

   std::cout << typeid(tIntStringMap::value_type).name() << std::endl;

   tIntStringMap::left_map& intToStr = numberMap.left;
   tIntStringMap::right_map& strToInt = numberMap.right;

   numberMap.left.erase(2);
   numberMap.right.erase("two");
   strToInt.erase("three");

   printTypes(numberMap);

   return 0;
}
