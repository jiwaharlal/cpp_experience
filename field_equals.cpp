#include <algorithm>
#include <boost/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/bind.hpp>
#include <boost/range/algorithm.hpp>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

//template <typename FirstType, typename SecondType>
//std::ostream& operator <<(std::ostream& out, const std::pair<FirstType, SecondType>& value)
//{
   //return out << "<" << value.first << ", " << value.second << ">";
//}

//std::ostream& operator <<(std::ostream& out, const std::pair<int, std::string>& value)
//{
   //return out << "<" << value.first << ", " << value.second << ">";
//}

template <typename T1, typename T2>
std::ostream& operator <<(std::ostream& out, const std::pair<T1, T2>& p)
{
   out << "<" << p.first << ", " << p.second << ">";

   return out;
}

//#define FieldEqueal(fieldName, value) \

template <typename MemberType, typename ReferenceValueType>
struct FieldEqual
{
   FieldEqual(MemberType member, const ReferenceValueType& refValue)
      : mRefValue(refValue)
      , mMember(member)
   {}

   template <typename ValueType>
   bool operator ()(const ValueType& value)
   {
      //return mRefValue == mMember(value);
      return mRefValue == boost::bind(mMember, value)();
   }

   ReferenceValueType mRefValue;
   MemberType mMember;
};

template <typename MemberType, typename ReferenceValueType>
FieldEqual<MemberType, ReferenceValueType> fieldEqual(MemberType member, const ReferenceValueType& refValue)
{
   return FieldEqual<MemberType, ReferenceValueType>(member, refValue);
}

int main()
{
   typedef std::vector<std::pair<int, std::string> > tIntStrList;
   tIntStrList intStrList = boost::assign::list_of
      (tIntStrList::value_type(23, "twentee three"))
      (tIntStrList::value_type(12, "twelve"))
      (tIntStrList::value_type(8, "eight"));

   BOOST_FOREACH(tIntStrList::const_reference p, intStrList)
   {
      std::cout << "<" << p.first << ", " << p.second << ">" << std::endl;
   }

   //boost::for_each(intStrList, std::cout << boost::lambda::_1 << std::endl);
   //std::copy(intStrList.begin(), intStrList.end(), std::ostream_iterator<tIntStrList::const_reference>(std::cout, "\n"));

   //boost::copy(intStrList, std::ostream_iterator<tIntStrList::const_reference>(std::cout, "\n"));
   //
   tIntStrList::const_iterator it = boost::find_if(intStrList, fieldEqual(&tIntStrList::value_type::first, 12));
   //tIntStrList::const_iterator it = boost::find_if(intStrList, boost::bind(std::equal, boost::bind(&tIntStrList::value_type::first, boost::lambda::_1));
   if (it != intStrList.end())
   {
      std::cout << "found " << *it << std::endl;
   }
   else
   {
      std::cout << "not found" << std::endl;
   }

   return 0;
}
