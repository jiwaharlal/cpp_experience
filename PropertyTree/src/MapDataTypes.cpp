#include <boost/fusion/adapted.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/mpl/range_c.hpp>
#include <iostream>
#include <string>

using namespace boost::fusion;

struct CGeoCoord
{
   CGeoCoord(double lat, double lon)
      : latitude(lat)
      , longitude(lon)
   {}

   bool operator =(const CGeoCoord& other) const
   {
      return latitude == other.latitude && longitude == other.longitude;
   }

   double latitude;
   double longitude;
};

struct CRoutePoint
{
   CRoutePoint(const CGeoCoord& c, const std::string& nar)
      : coord(c)
      , narrative(nar)
   {}

   CGeoCoord coord;
   std::string narrative;
};

BOOST_FUSION_ADAPT_STRUCT(
      CGeoCoord,
      (double, latitude)
      (double, longitude)
      )

BOOST_FUSION_ADAPT_STRUCT(
      CRoutePoint,
      (CGeoCoord, coord)
      (std::string, narrative)
      )

struct Printer
{
   template <typename T>
   void operator ()(const T& value) const
   {
      std::cout << value << std::endl;
   }
};

//template <typename T>
//typename boost::disable_if<traits::is_sequence<T>, void>::type
//printNameValue(const std::string& name, const T& data)
//{
   //std::cout << name << " : ";
   //print
//}

template <typename Seq>
class NameValuePrinter
{
public:
   NameValuePrinter(const Seq& sequence, std::ostream& out)
      : mSeq(sequence)
      , mOut(out)
   {
   }

   template <typename Index>
   void operator() (Index) const
   {
      const std::string& fieldName =
         extension::struct_member_name<Seq, Index::value>::call();
      mOut << std::endl << fieldName << std::string(" : ") << at<Index>(mSeq);
   }

private:
   const Seq& mSeq;
   std::ostream& mOut;
};

template <typename T>
typename boost::enable_if<traits::is_sequence<T>, std::ostream&>::type
operator <<(std::ostream& out, const T& value)
{
   typedef boost::mpl::range_c<size_t, 0, boost::fusion::result_of::size<T>::value> tIndices;
   for_each(tIndices(), NameValuePrinter<T>(value, out));

   return out;
}

int main()
{

   CGeoCoord c(3.4, 5.6);
   for_each(c, Printer());

   CRoutePoint p(c, "hello");

   typedef boost::mpl::range_c<size_t, 0, result_of::size<CGeoCoord>::value> tIndices;

   //for_each(tIndices(), NameValuePrinter<CGeoCoord>(c));
   std::cout << c << std::endl;
   std::cout << p << std::endl;

   return 0;
}
