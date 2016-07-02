#include <iostream>
#include <boost/bimap.hpp>
#include <iterator>
#include <algorithm>
#include <string>
#include <boost/lambda/lambda.hpp>
#include <typeinfo>

typedef boost::bimap<int, std::string> tIntStringMap;

template <typename FirstType, typename SecondType>
std::ostream& operator <<(std::ostream& out, typename boost::bimap<FirstType, SecondType>::const_reference p)
{
   //out << "<" << p.first << ", " << p.second;
   return out;
}

//template <typename FirstType, typename SecondType>
//std::ostream& operator <<(std::ostream& out, typename boost::bimap<FirstType, SecondType>::left_map::const_reference p)
//{
   //out << "<" << p.first << ", " << p.second << ">";
   //return out;
//}

template <typename FirstType, typename SecondType>
void printTypes(const boost::bimap<FirstType, SecondType>& bm)
{
   std::cout << "First: " << typeid(bm.left).name() << std::endl;
}

int main(int, char**)
{
   tIntStringMap numberMap;

   numberMap.insert(tIntStringMap::value_type(1, "one"));
   numberMap.insert(tIntStringMap::value_type(2, "two"));
   numberMap.insert(tIntStringMap::value_type(3, "three"));

   std::cout << typeid(tIntStringMap::value_type).name() << std::endl;

   tIntStringMap::left_map& intToStr = numberMap.left;
   tIntStringMap::right_map& strToInt = numberMap.right;

   numberMap.left.erase(2);
   numberMap.right.erase("two");
   strToInt.erase("three");

   std::cout << "map size: " << numberMap.size() << std::endl;

   //std::copy(numberMap.begin(), numberMap.end(), std::ostream_iterator<tIntStringMap::const_reference>(std::cout, "\n"));
   //std::for_each(numberMap.begin(), numberMap.end(), std::cout << boost::lambda::_1);
   for (tIntStringMap::const_iterator it = numberMap.begin(); it != numberMap.end(); ++it)
   {
      //std::cout << *it << std::endl;
   }

   //tIntStringMap::left_map::const_reference el = *(intToStr.begin());
   for (auto el: numberMap)
   {
      std::cout << el.first;
   }

   std::cout << typeid(tIntStringMap::left_map).name() << std::cout;

   printTypes(numberMap);

   return 0;
}
