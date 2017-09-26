#include <boost/range/algorithm.hpp>
#include <boost/range/irange.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/next_prior.hpp>
#include <iostream>
#include <list>
#include <vector>

struct Type1{};
struct Type2{};
struct Type3{};
struct Type4{};

void convert(const Type1&, Type2& dst)
{
   dst = Type2();
}

void convert(const Type3&, Type4& dst)
{
   dst = Type4();
}

//template <typename SrcType>
//struct Converter
//{
   //template <typename DstType>
   //DstType convert(const SrcType& src)
   //{
      //DstType result;
      //convert(src, result);
      //return result;
   //}
//};

template <typename DstType>
struct Converter
{
   template <typename SrcType>
   DstType operator ()(const SrcType& src)
   {
      DstType dst;
      convert(src, dst);
      return dst;
   }
};

struct A {};

struct B : public A {};

int main(int, char**)
{
   boost::copy(boost::irange(0, 10), std::ostream_iterator<int>(std::cout, ", "));
   std::cout << std::endl;

   boost::shared_ptr<B> pb(new B());
   boost::shared_ptr<A> pa = pb;

   typedef std::list<int> tIntList;
   tIntList ints;
   boost::copy(boost::irange(0, 10), std::back_inserter(ints));
   for (tIntList::iterator it = ints.begin(); it != ints.end(); ++it)
   {
      if (*it % 2 == 0)
      {
         //--it;
         //ints.erase(boost::next(it));
         ints.erase(it--);
      }
   }
   boost::copy(ints, std::ostream_iterator<int>(std::cout, ", "));
   std::cout << std::endl;

   std::vector<int32_t> non_unique{1, 2, 2, 3, 4, 4, 4, 5, 6, 7, 7, 7};
   auto unique_range = boost::unique(non_unique);
   auto adjacent_range = boost::adjacent_find(non_unique);

   std::cout << "Adjacent range:\n";
   boost::copy(adjacent_range, std::ostream_iterator<int32_t>(std::cout, ", "));
   std::cout << std::endl;

   std::cout << "Unique range:\n";
   boost::copy(unique_range, std::ostream_iterator<int32_t>(std::cout, ", "));
   std::cout << std::endl;

   return 0;
}
