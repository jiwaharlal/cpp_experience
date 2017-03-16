#include <boost/make_shared.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/list.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_index.hpp>
#include <boost/variant.hpp>
#include <iostream>
#include <vector>

struct Position
{
   double lat;
   double lon;
   double direction;
   std::vector<double> someMoreStuff;
};

struct TypeSizePrinter
{
   template <typename T>
   void operator()(const T&)
   {
      std::cout << "Size of " << boost::typeindex::type_id<T>().pretty_name()
               << " : " << sizeof(T) << std::endl;
   }
};

int main()
{
   typedef boost::mpl::list
      <
         std::vector<int>,
         boost::optional<std::vector<int> >,
         Position,
         boost::optional<Position>,
         int,
         boost::optional<int>,
         short int,
         boost::optional<short int>,
         char,
         boost::optional<char>
      > tOpList;

   typedef typename boost::make_variant_over<tOpList>::type tVariant;

   //tVariant v(std::vector<int>(10, 10));

   std::cout << "Size of variant : " << sizeof(tVariant) << "\n";

   boost::mpl::for_each<tOpList>(TypeSizePrinter());

   boost::optional<int> i; //= 5;
   std::cout << std::boolalpha << i.is_initialized() << std::endl;

   std::cout << i << std::endl;
   //std::cout << i.get() << std::endl;

   boost::shared_ptr<int> pi = boost::make_shared<int>(22);
   boost::shared_ptr<const int> cpi = pi;

   std::cout << "use count: " << pi.use_count() << "\n";
   std::cout << "int value: " << *pi << "\n";
   *pi += 1;

   std::cout << "int value: " << *cpi << "\n";
   //*cpi += 2;

   boost::shared_ptr<int> emptyPi;
   int& emptyRef = *emptyPi;

   return 0;
}
