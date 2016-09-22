#include <boost/mpl/list.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/type_index.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <iostream>
#include <string>

typedef boost::mpl::vector<int, double> tNumericList;
typedef boost::mpl::vector<char, std::string> tStringList;

typedef boost::mpl::copy
<
   tNumericList,
   boost::mpl::back_inserter<tStringList>
>::type tFullList;

struct Printer
{
   template <typename T>
   void operator ()(T&)
   {
      std::cout << "Found type: " << boost::typeindex::type_id<T>().pretty_name() << std::endl;
   }
};

int main()
{
   boost::mpl::for_each<tFullList>(Printer());

   return 0;
}
