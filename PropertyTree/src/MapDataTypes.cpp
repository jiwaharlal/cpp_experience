#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/phoenix.hpp>
#include <boost/variant.hpp>
#include <iostream>
#include <string>
#include <vector>

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

struct IPropertyItem;
typedef boost::shared_ptr<IPropertyItem> tPropPtr;

struct IPropertyItem
{
   virtual ~IPropertyItem() {}
   virtual std::string getValue() const = 0;
   virtual bool trySetValue(const std::string newValue) = 0;
   virtual std::string getName() const = 0;

   std::vector<tPropPtr> children;
   std::vector<std::string> options;
};

struct PropertyItemBase: public IPropertyItem
{
   PropertyItemBase(const std::string& name)
      : mName(name)
   {}

   virtual std::string getName() const
   {
      return mName;
   }

private:
   const std::string mName;
};

template <typename T, typename EditFunctorType>
struct TPropertyItemImpl: public PropertyItemBase
{
   typedef boost::function<T&()> tAccessorFuncType;

   TPropertyItemImpl(
         const std::string& name,
         const tAccessorFuncType& accessor,
         const boost::function<void()>& notifier,
         const EditFunctorType& editFunctor)
      : PropertyItemBase(name)
      , mAccessor(accessor)
      , mNotifier(notifier)
      , mEditFunctor(editFunctor)
   {}

   virtual std::string getValue() const
   {
      return boost::lexical_cast<std::string>(mAccessor());
   }

   virtual bool trySetValue(const std::string newValue)
   {
      try
      {
         mEditFunctor(mAccessor, boost::lexical_cast<T>(newValue));
         mNotifier();

         return true;
      }
      catch (...)
      {
         std::cout << "Error converting value " << newValue << std::endl;

         return false;
      }
   }

   tAccessorFuncType mAccessor;
   boost::function<void()> mNotifier;
   EditFunctorType mEditFunctor;
};

template <typename T>
struct TPropertyNodeImpl: public PropertyItemBase
{
   TPropertyNodeImpl(const std::string& name)
      : PropertyItemBase(name)
   {}

   virtual std::string getValue() const
   {
      return "Node";
   }

   virtual bool trySetValue(const std::string)
   {
      return false;
   }
};

struct Printer
{
   template <typename T>
   void operator ()(const T& value) const
   {
      std::cout << value << std::endl;
   }
};

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

//template <typename T>
//typename boost::enable_if<traits::is_sequence<T>, std::ostream&>::type
//operator <<(std::ostream& out, const T& value)
//{
   //typedef boost::mpl::range_c<size_t, 0, boost::fusion::result_of::size<T>::value> tIndices;
   //for_each(tIndices(), NameValuePrinter<T>(value, out));

   //return out;
//}

template <typename T, typename EditFunctorType>
typename boost::disable_if<traits::is_sequence<T>, tPropPtr>::type
generateProperty(
      const std::string& name,
      boost::function<T&()> accessor,
      boost::function<void()> notifier,
      const EditFunctorType& editFunctor)
{
   return boost::make_shared<TPropertyItemImpl<T, EditFunctorType> >(name, accessor, notifier, editFunctor);
}

template <typename T, typename SeqType, typename Index>
T& getAt(SeqType& seq)
{
   return at<Index>(seq);
}

template <typename Seq, typename EditFunctorType>
class PropertyGenerator
{
public:
   PropertyGenerator(
         tPropPtr node,
         Seq& seq,
         const boost::function<void()>& notifier,
         const EditFunctorType& editFunctor)
      : mNode(node)
      , mSeq(seq)
      , mNotifier(notifier)
      , mEditFunctor(editFunctor)
   {}

   template <typename Index>
   void operator() (Index) const
   {
      const std::string& fieldName =
         extension::struct_member_name<Seq, Index::value>::call();

      typedef typename boost::remove_reference<typename result_of::at<Seq, Index>::type>::type FieldType;

      tPropPtr item = generateProperty<FieldType, EditFunctorType>(
            fieldName,
            boost::bind(&getAt<FieldType, Seq, Index>, boost::ref(mSeq)),
            mNotifier,
            mEditFunctor);

      mNode->children.push_back(item);
   }

private:
   tPropPtr mNode;
   Seq& mSeq;
   boost::function<void()> mNotifier;
   EditFunctorType mEditFunctor;
};

template <typename T, typename EditFunctorType>
typename boost::enable_if<traits::is_sequence<T>, tPropPtr>::type
generateProperty(
      const std::string& name,
      boost::function<T&()> accessor,
      boost::function<void()> notifier,
      const EditFunctorType& editFunctor)
{
   tPropPtr node = boost::make_shared<TPropertyNodeImpl<T> >(name);

   typedef boost::mpl::range_c<size_t, 0, boost::fusion::result_of::size<T>::value> tIndices;
   for_each(tIndices(), PropertyGenerator<T, EditFunctorType>(node, accessor(), notifier, editFunctor));

   return node;
}

void notifier()
{
   std::cout << "Notified" << std::endl;
}

std::ostream& operator <<(std::ostream& out, const IPropertyItem& propItem)
{
   out << propItem.getName() << " : " << propItem.getValue() << std::endl;
   BOOST_FOREACH(const tPropPtr& p, propItem.children)
   {
      out << *p;
   }

   return out;
}

struct SimpleEditFunctor
{
   template <typename T>
   void operator ()(boost::function<T&()> accessor, const T& newValue)
   {
      accessor() = newValue;
   }
};

int main()
{

   CGeoCoord c(3.4, 5.6);
   for_each(c, Printer());

   CRoutePoint p(c, "hello");

   //tPropPtr root = generateProperty<CGeoCoord>("coord", boost::phoenix::ref(c), &notifier);
   tPropPtr root = generateProperty<CRoutePoint>("point", boost::phoenix::ref(p), &notifier, SimpleEditFunctor());

   std::cout << *root;

   root->children[0]->children[0]->trySetValue("6.8");

   std::cout << *root << std::endl;
   std::cout << p.coord.latitude << ", " << p.coord.longitude << std::endl;

   //int i;
   //tPropPtr pi = generateProperty<int>("index", boost::phoenix::ref(i), &notifier);

   typedef boost::mpl::range_c<size_t, 0, result_of::size<CGeoCoord>::value> tIndices;

   //for_each(tIndices(), NameValuePrinter<CGeoCoord>(c));
   //std::cout << c << std::endl;
   //std::cout << p << std::endl;

   return 0;
}
