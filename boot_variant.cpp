#include <boost/variant.hpp>
#include <iostream>
#include <string>
#include <boost/variant/static_visitor.hpp>
#include <boost/tuple/tuple.hpp>

//typedef boost::mpl::list<int, boost::tuple<double, std::string> > tList;
typedef boost::variant<int, boost::tuple<double, std::string> > tVariant;
//typedef boost::variant<int, double> tVariant;

struct ArgumentExtractor: public boost::static_visitor<>
{
   void operator ()(int)
   {
      std::cout << "int visited" << std::endl;
   }

   //void operator ()(boost::tuple<double, std::string>&)
   //{
      //std::cout << "tuple visited" << std::endl;
   //}

   void operator ()(double)
   {
      std::cout << "double visited" << std::endl;
   }

   template <class T>
   void operator ()(const T&)
   {
   }

   template <typename T1, typename T2>
   void operator ()(const boost::tuple<T1, T2>& t)
   {
      std::cout << "Tuple: (" << boost::get<0>(t) << ", " << boost::get<1>(t) << ")" << std::endl;
   }
};

int main()
{
   tVariant v(boost::make_tuple(1.1, "hello"));
   //tVariant v(2);

   ArgumentExtractor ext;
   boost::apply_visitor(ext, v);

   return 0;
}
