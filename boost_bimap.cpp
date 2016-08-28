#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/assign.hpp>
#include <boost/bimap.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/phoenix.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm/find_end.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <iostream>
#include <iterator>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

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
   tIntStringMap numberMap = boost::assign::list_of<tIntStringMap::value_type>
      (1, "one") (2, "two") (3, "three");

   //numberMap.insert(tIntStringMap::value_type(1, "one"));
   //numberMap.insert(tIntStringMap::value_type(2, "two"));
   //numberMap.insert(tIntStringMap::value_type(3, "three"));

   std::vector<int> v;

   std::string optionsStr = boost::algorithm::join(boost::adaptors::values(numberMap.left), ", ");
   std::cout << optionsStr << std::endl;

   boost::assign::insert(numberMap)(4, "four");

   //tIntStringMap::relation::left_pair::left_value_type
   boost::transform(
         numberMap.left,
         std::back_inserter(v),
         boost::bind(&tIntStringMap::left_value_type::first, _1));

   //std::stringstream options;
   std::vector<std::string> optionList;

   boost::transform(
         numberMap.left,
         std::back_inserter(optionList),
         boost::bind(&tIntStringMap::left_value_type::second, _1));
         //boost::bind(&tIntStringMap::left_value_type::second, boost::phoenix::placeholders::arg1));

   //boost::transform(
         //numberMap.left,
         //std::ostream_iterator<std::string>(options, ", "),
         //boost::bind(&tIntStringMap::left_value_type::second, _1));

   //std::string clean = boost::trim_right_copy_if(options.str(), boost::is_any_of(" ,"));

   ////std::cout << clean << std::endl;
   //std::cout << boost::algorithm::join(optionList, ", ") << std::endl;

   using boost::algorithm::join;
   using boost::adaptors::values;
   std::string options = join(values(numberMap.left), ", ");

   using boost::phoenix::placeholders::arg1;
   int someValue = 42;
   using boost::adaptors::keys;
   boost::transform(keys(numberMap.left), std::back_inserter(v), arg1 * someValue);

   std::cout << options << std::endl;

   BOOST_FOREACH(typename tIntStringMap::left_reference intStrPair, numberMap.left)
   {
      v.push_back(intStrPair.first);
   }

   return 0;
}
