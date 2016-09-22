#include <boost/core/enable_if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/back_inserter.hpp>
#include <boost/mpl/copy.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/ref.hpp>
#include <boost/type_index.hpp>
#include <iostream>
#include <string>

typedef boost::mpl::vector<int, double> tNumericList;
typedef boost::mpl::vector<char, std::string> tStringList;

typedef boost::mpl::copy
<
   tNumericList,
   boost::mpl::back_inserter<tStringList>
>::type tFullList;

struct NonConstructable
{
private:
   NonConstructable(int);
};

typedef boost::mpl::list<int, double, NonConstructable> tNumericTypesList;
typedef boost::mpl::list<std::string, char> tStringTypeList;

template <typename T>
struct Wrap{};

struct Printer
{
   template <typename T>
   void operator()(Wrap<T>)
   {
      operator()<T>();
   }

   template <typename T>
   void operator ()()
   {
      std::cout << "Found type: " << boost::typeindex::type_id<T>().pretty_name() << std::endl;
   }
};

int main()
{
   boost::mpl::for_each<tFullList, Wrap<boost::mpl::placeholders::_1> >(Printer());

   return 0;
}
